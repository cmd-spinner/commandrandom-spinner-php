#include<io.h>
#include<fstream>
#include<string>
#include<regex>
#include<vector>
#include<direct.h>

#include "AST.h"
#include "callgraph.h"
//#include "classTmap.h"
#include"tmap.h"
#include "CFGtattr.h"
#include "Tout.h"

using namespace std;

std::vector<callgraph*> Callgraph;
std::vector<classTmap*> ClassTmap;
std::vector<unique_ptr<ASTUnit>> astUnit;
std::vector<string> files;
Ttable t_table;
std::vector<std::vector<XYJ_table*>*> xyj_table;
string folder_path = "NULL";

void get_file(string path, std::vector<string>& all_file);
void print_file(const std::vector<string> files);
bool is_syslib(string rd);

int main(int argc, char *argv[]) {
	//获取目录下所有.cpp、.c文件所生成的.ast文件
	get_file(argv[1], files);

	int func_num = 0;
	int class_num = 0;

	std::vector<callgraph*>::iterator it_callgraph, it_call_last = Callgraph.end();
	int file_size = files.size();
	for (int i = 0; i < file_size; i++)
	{
		//获取ASTUnit
		FileSystemOptions opts;
		IntrusiveRefCntPtr<DiagnosticsEngine> Diags = CompilerInstance::createDiagnostics(new DiagnosticOptions());
		CompilerInstance compiler;
		unique_ptr<ASTUnit> AU = ASTUnit::LoadFromASTFile(files[i], compiler.getPCHContainerReader(), Diags, opts);
		//保存ASTUnit
		astUnit.insert(astUnit.end(), move(AU));
		int astUnit_size = astUnit.size();
		ASTContext &context = (astUnit[astUnit_size - 1])->getASTContext();

		//扫描AST获取全局变量定义
		//ASTCXXRecordLoad loadClass;
		//loadClass.HandleTranslationUnit(context);
		//std::vector<CXXRecordDecl*>   cxxrds = loadClass.getClassDecl();

		//扫描AST获取类定义
		ASTCXXRecordLoad loadClass;
		loadClass.HandleTranslationUnit(context);
		std::vector<CXXRecordDecl*>   cxxrds = loadClass.getClassDecl();

		if (cxxrds.size() > 0)//类的数目判断
		{
			std::vector<CXXRecordDecl*>::iterator rd_it, rd_it_end = cxxrds.end();
			for (rd_it = cxxrds.begin(); rd_it != rd_it_end; rd_it++)
			{
				//是否为库类
				if (is_syslib((*rd_it)->getQualifiedNameAsString()) == true)
					continue;

				//在ClassTmap中加入新发现的class
				if (if_find_class(ClassTmap, *rd_it))
				{
					continue;//已经有了，不再分析
				}
				classTmap* curClass = new classTmap();
				curClass->setCXXRecordDecl(*rd_it);
				ClassTmap.push_back(curClass);
				class_num++;

				//添加类方法decl(仅仅为其创建callgraph函数，添加caller等操作之后统一进行)
				ASTCXXMethodDeclLoad loadClassMethod;
				std::vector<CXXMethodDecl *> cxxmds;
				std::vector<CXXMethodDecl *>::iterator it_cxxmds;
				//std::cout << (*rd_it)->getQualifiedNameAsString() << "\n";
				loadClassMethod.TraverseDecl(*rd_it);
				cxxmds = loadClassMethod.getCXXMethodDecl();

				if (cxxmds.size() > 0)//类方法数目
				{
					for (it_cxxmds = cxxmds.begin(); it_cxxmds != cxxmds.end(); it_cxxmds++)
					{
						curClass->addMethod(*it_cxxmds);
						callgraph* tempClassMethodNode = new callgraph(*it_cxxmds);
						Callgraph.insert(Callgraph.end(), tempClassMethodNode);
						tempClassMethodNode->set_ASTContext(&context);
						func_num++;
						//std::cout << (*it_cxxmds)->getQualifiedNameAsString() << "\n";

					}
				}

				//添加类变量decl(类型为fieldDecl*)
				ASTFieldDeclLoad loadClassVar;
				std::vector<FieldDecl*> fds;
				std::vector<FieldDecl*>::iterator it_fds;
				loadClassVar.TraverseDecl(*rd_it);
				fds = loadClassVar.getFieldDecl();
				if (fds.size() > 0)
				{
					for (it_fds = fds.begin(); it_fds != fds.end(); it_fds++)
					{
						FieldDecl* fd_temp = *it_fds;
						if (fd_temp->getType()->isPointerType())
						{
							curClass->addVar(fd_temp, TYPE_POINTER);
							//curClass->getMap()->insert(); TYPE_POINTER;
						}
						else if (fd_temp->getType()->isStructureOrClassType())
						{
							curClass->addVar(fd_temp, TYPE_CLASS);
						}
						else
						{
							curClass->addVar(fd_temp, TYPE_VARIABLE);
						}
						//std::cout << (*it_fds)->getQualifiedNameAsString() << "\n";
					}
				}
			}
		}//cxxrds finished

		//扫描AST获取vector::<functionDecl>
		ASTFunctionLoad load;
		load.HandleTranslationUnit(context);
		std::vector<FunctionDecl*>  func = load.getFunctions();

		std::vector<FunctionDecl*>::iterator it_func_decl;

		//add cur->callee
		if (func.size() > 0)
		{
			//为functionDecl添加callgraph
			callgraph* tempCallNode;
			for (it_func_decl = func.begin(); it_func_decl != func.end(); it_func_decl++)
			{
				//是否为库函数
				if (is_syslib((*it_func_decl)->getQualifiedNameAsString()) == true)
					continue;

				//重复的functiondecl
				if (if_find_function(Callgraph, (*it_func_decl)) == true)
					continue;
				tempCallNode = new callgraph(*it_func_decl);
				Callgraph.push_back(tempCallNode);
				tempCallNode->set_ASTContext(&context);
				func_num++;
			}
		}
	}
	//astUnit over

	//std::vector<callgraph*>::iterator 
	it_callgraph = Callgraph.begin();
	it_call_last = Callgraph.end();
	std::vector<CallExpr*>  callExpr;
	std::vector<CallExpr*>::iterator it_call_expr;
	callgraph* tempCallgraph;
	//为所有方法添加函数调用关系

	FunctionDecl* cur;
	for (; it_callgraph != it_call_last; it_callgraph++)
	{
		cur = (*it_callgraph)->getCur();

		//设置函数返回值类型
		if (cur->getReturnType()->isPointerType())
			(*it_callgraph)->get_return()->setType(TYPE_POINTER);
		//	else if (cur->getReturnType()->isStructureOrClassType())
		//	(*it_callgraph)->getReturn()->setType(TYPE_CLASS);
		else
			(*it_callgraph)->get_return()->setType(TYPE_VARIABLE);

		ASTCallExprLoad load2;
		load2.TraverseStmt(cur->getBody());
		callExpr = load2.getCallExprs();
		if (callExpr.size() > 0)
		{
			//添加调用的函数callee
			for (it_call_expr = callExpr.begin(); it_call_expr != callExpr.end(); it_call_expr++)
			{
				FunctionDecl* callee = (*it_call_expr)->getDirectCallee();
				if (callee == NULL || (*it_callgraph)->is_callee(callee) == true)
				{
					continue;
				}
				(*it_callgraph)->addCallee(callee, *it_call_expr);
				//std::cout << (*it_callgraph)->getCur()->getQualifiedNameAsString() << " -> " << (*it_call_expr)->getDirectCallee()->getQualifiedNameAsString() << std::endl;
				tempCallgraph = findById(Callgraph, callee->getQualifiedNameAsString());
				if (tempCallgraph == NULL || tempCallgraph->is_caller((*it_callgraph)->getCur()))
				{
					continue;
				}
				else
				{
					tempCallgraph->addCaller((*it_callgraph)->getCur());
				}
			}
			//添加调用本函数的函数caller
		}
		//把参数加入Tmap
		int paramNum = cur->getNumParams();
		//VarDecl* parmTemp;
		if (paramNum > 0)
		{
			for (int i = 0; i < paramNum; i++)
			{
				ParmVarDecl* parm_temp = cur->getParamDecl(i);
				(*it_callgraph)->addParam(parm_temp);
				CTmap* map = &(*it_callgraph)->getCTmap();

				//设置参数的污染信息相关的初始化
				if (parm_temp->getType()->isPointerType())
				{
					map->setType(parm_temp, TYPE_POINTER);
					//map->ptr_attr_set(parm_temp, UNTAINTED, NULL);
				}
				else if (parm_temp->getType()->isStructureOrClassType())
				{
					map->setType(parm_temp, TYPE_CLASS);
					CXXRecordDecl* t = parm_temp->getType()->getAsCXXRecordDecl();
					classTmap* ct = getClassTmap(t);
					classTmap* new_ct = new classTmap();
					new_ct->classCopy(ct);
					map->classmember_attr_set(parm_temp, new_ct);
				}
				else
				{
					map->setType(parm_temp, TYPE_VARIABLE);
					map->var_attr_set(parm_temp, RELATED, parm_temp);
				}
			}
		}

		//把中间变量加入Tmap
		ASTVarDeclLoad loadVar;
		loadVar.TraverseStmt(cur->getBody());
		std::vector<VarDecl*> varList = loadVar.getVariables();
		for (std::vector<VarDecl*>::iterator var_it = varList.begin(); var_it != varList.end(); var_it++)
		{
			//std::cout << (*varIt)->getQualifiedNameAsString() << "\n";
			VarDecl* var_temp = *var_it;
			(*it_callgraph)->addVar(var_temp);
			CTmap* map = &(*it_callgraph)->getCTmap();

			//设置变量的污染信息相关的初始化
			if (var_temp->getType()->isPointerType())
			{
				map->setType(var_temp, TYPE_POINTER);
				//	map->ptr_attr_set(var_temp, UNTAINTED, NULL);
			}
			else if (var_temp->getType()->isStructureOrClassType())
			{
				map->setType(var_temp, TYPE_CLASS);
				CXXRecordDecl* t = var_temp->getType()->getAsCXXRecordDecl();
				classTmap* ct = getClassTmap(t);
				classTmap* new_ct = new classTmap();
				new_ct->classCopy(ct);
				map->classmember_attr_set(var_temp, new_ct);
			}
			else
			{
				map->setType(var_temp, TYPE_VARIABLE);
				map->var_attr_set(var_temp, UNTAINTED, NULL);
			}
		}
	}

	//std::vector<FunctionDecl*> ringVector;
	//getRing(Callgraph, 0, ringVector);

	//resetIfCheck(Callgraph);
	//ifcheck(Callgraph, *Callgraph.begin());

	printCallGraph();

	//printClassTmap(ClassTmap);

	cout << "class num：\t" << class_num << "\n";
	cout << "function num：\t" << func_num << "\n";
	std::vector<callgraph*>::iterator it3;

	it3 = Callgraph.begin();
	for (; it3 != Callgraph.end(); it3++)
	{
		if ((*it3)->getCur()->getQualifiedNameAsString().compare(string("main")) == 0)
		{
			resetIfCheck(Callgraph);
			ringCheck(Callgraph, *it3);
			break;
		}
	}

	printCallGraph();

	it3 = Callgraph.begin();
	for (; it3 != Callgraph.end(); it3++)
	{
		if ((*it3)->getCur()->getQualifiedNameAsString().compare(string("main")) == 0)
		{
			checkCFG(*(*it3)->get_cfg(), (*it3)->getCTmap(), *it3);
			MsgOutput2Xml(*it3, t_table);
			t_table.XMLout(folder_path+string("\\warning.xml"));
			break;
		}
	}

	//FunctionDecl* ddd=Callgraph[0]->getCur();
	//Stmt* sss=ddd->getBody();
	//ASTContext &aaaa=astUnit[1]->getASTContext();

	//string s(sss->getLocStart().printToString(aaaa.getSourceManager()));
	return 0;
}

void get_file(string path, std::vector<string>& all_files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	//基础空串
	string p;
	hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo);
	if (folder_path.compare(string("NULL")) == 0)
	{
		folder_path = p.assign(path);
	}
	if (hFile == -1)
	{
		cout << "文件夹不存在" << endl;
	}
	else
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))  //比较文件类型是否是文件夹
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					get_file(p.assign(path).append("\\").append(fileinfo.name), all_files);
				}
			}
			else if (regex_match(fileinfo.name, regex("(.*)(.ast)")))
			{
				all_files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				cout << p << endl;
			}
			else
			{
				;
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
		_findclose(hFile);
	}
}

void print_file(const vector<string> files)
{
	int size = files.size();
	for (int i = 0; i < size; i++)
	{
		cout << files[i] << endl;
	}
}

bool is_syslib(string rd)
{
	if (regex_match(rd, regex("(std::)(.*)")))
		return true;
	else if (regex_match(rd, regex("(stdext::)(.*)")))
		return true;
	else if (regex_match(rd, regex("(type_info)(.*)")))
		return true;
	else
		return false;
}
