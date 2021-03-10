#ifndef CALLGRAPH_H
#define CALLGRAPH_H

class CFGInOut;

#include<vector>
#include "AST.h"
#include "CFGtattr.h"
#include "TCI.h"
#include "Tout.h"

using namespace std;
using namespace clang;

extern vector<callgraph*> Callgraph;
extern Ttable t_table;
extern std::vector<std::vector<XYJ_table*>*> xyj_table;

typedef enum
{
	common, inclass 
}methodType;

//函数调用关系图
class callgraph{
public:
	callgraph(FunctionDecl* f1);
	callgraph(FunctionDecl* f1, FunctionDecl* f2);
	~callgraph();
	//调用cur的函数
	std::vector<FunctionDecl*>& getCaller();		
	//被cur调用的函数
	std::vector<FunctionDecl*>& getCallee();		
	FunctionDecl* getCur();
	int getCallerNum();
	int getCalleeNum();
	void addCaller(FunctionDecl* otherFD);
	void addCallee(FunctionDecl* otherFD,CallExpr* expr);
	void delCallee(FunctionDecl* otherFD);
	void delCaller(FunctionDecl* otherFD);
	void changeMethodType();
	void setRoot(VarDecl* r);
	void setClass(CXXRecordDecl* rd);
	CXXRecordDecl* getClass();
	VarDecl* getRoot();
	methodType getMethodType();
	bool is_caller(const FunctionDecl* fd);
	bool is_callee(const FunctionDecl* fd);

public:
	std::unique_ptr<CFG>& get_cfg();
	void print_cfg();
	CTmap& getCTmap();
	void addParam(VarDecl* vd);
	void addVar(VarDecl* vd);
	int getParamNum();
	int getVarNum();
	Tainted_Attr* get_return();
	void set_return(Tainted_Attr* temp);
	void set_if_check_cfg();
	bool get_if_check_cfg();
	unsigned get_return_relation();
	bool set_return_relation(int i);
	int get_param_no(const VarDecl* vd);
	void set_ASTContext(ASTContext* astcontext);
	SourceManager& getSourceManager();
	ASTContext* getASTContext();
	std::map<FunctionDecl*, CallExpr*>& get_call_map();

public:
	std::map<clang::CFGBlock *, CFGInOut> block_io_map;
	vector<TCI*> TCI_list;
	vector<TCI*> TCI_list_call;
	int ifCheck;

private:
	//方法的类型
	methodType type;
	//类方法所属的实例
	VarDecl* root;
	CXXRecordDecl* classDecl;
	std::vector<FunctionDecl*> caller;
	FunctionDecl* cur;
	std::vector<FunctionDecl*> callee;
	std::map<FunctionDecl*, CallExpr*> call_map;
	std::unique_ptr<CFG> cfg;
	int callerNum, calleeNum;
	vector<const VarDecl*> map_param;
	CTmap* map;
	int paramNum;
	int varNum;
	Tainted_Attr* return_tattr;
	unsigned return_relation;
	bool if_check_cfg;
	ASTContext* astcontext;
};

callgraph* findById(std::vector<callgraph*> Callgraph, std::string id);
void ringCheck(std::vector<callgraph*> cg, callgraph* t);
void resetIfCheck(std::vector<callgraph*>Callgraph);
void getRing(std::vector<callgraph*>& Callgraph, int n, std::vector<FunctionDecl*>& ringVector);
void printCallGraph();

bool if_find_function(std::vector<callgraph*> Callgraph,FunctionDecl* fd);
#endif
