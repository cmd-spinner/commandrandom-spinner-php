#ifndef _TMAP_H_
#define _TMAP_H_

#include <iostream>
#include <map>
#include <vector>

#include "clang/Frontend/ASTUnit.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/FileSystemOptions.h"
#include "clang/AST/RecursiveASTVisitor.h"

//#include "Tout.h"

using namespace std;
using namespace clang;
using namespace llvm;

class callgraph;
class CTmap;
class classTmap;
extern std::vector<classTmap*> ClassTmap;

//污染情况
typedef enum{
	TAINTED,
	UNTAINTED,
	RELATED
}e_tattr;

//变量的类型，供tmap使用
typedef enum{
	TYPE_VARIABLE,	//变量
	TYPE_CLASS,		//类
	TYPE_POINTER,	//指针
	TYPE_UNKNOWN	//未知
}VarDeclType;

typedef enum{
	U2T,
	T2U,
	OTHER
}MessageType;

void printClassTmap(std::vector<classTmap*> CT);
bool if_find_class(std::vector<classTmap*>ClassTmap, CXXRecordDecl* rd);
classTmap* getClassTmap(CXXRecordDecl* rd);

//变量的污染属性
class Tainted_Attr
{
private:
	VarDeclType type;

	//污染属性
	e_tattr attr;
	//污染与哪些变量相关
	set<const VarDecl *> relation;

	//指向该指针指向的位置的污染状态
	Tainted_Attr *ptrAttr;
	bool is_temp;
	//指向该类的实例的map
	classTmap *ptrClassDecl;

public:
	Tainted_Attr();
	Tainted_Attr(VarDeclType mytype, classTmap *ct);
	Tainted_Attr(Tainted_Attr& b);
	~Tainted_Attr();

	/*获取相关的函数*/

	VarDeclType getType();
	e_tattr getVariableAttr();
	set<const VarDecl *> *getVariableRelation();
	classTmap *getClassDecl();
	Tainted_Attr *getPointerAttr();
	bool getistemp();

	/*调试相关的函数*/
	void output();
	void copy(Tainted_Attr *p);

	/*属性设置相关的函数*/

	void settemp(bool b);
	void var_attr_set(e_tattr a, const VarDecl *r);
	void var_attr_set(e_tattr a, set<const VarDecl *> r);
	void class_attr_set(e_tattr a, const VarDecl *r, Expr *ptrExp);
	void classmember_set(classTmap *ct);
	void pointer_attr_set(e_tattr a, const VarDecl *r);
	void setPointer(Tainted_Attr *pt);
	void setType(VarDeclType tp);
	void unionAttr(Tainted_Attr &a, Tainted_Attr &b);
	void unionAttr(Tainted_Attr &a);
	bool compareAttr(Tainted_Attr &ta);
};

//封装了C++ map模板的污染表类 
class CTmap
{
private:
	map<const VarDecl *, Tainted_Attr *> tmap;
public:
	CTmap();
	CTmap(CTmap& b);
	~CTmap();
	void output();
	//void output2xml(callgraph *cg);
	void CopyMap(CTmap& b);
	void insert(const VarDecl *p);
	void del(const VarDecl *p);
	Tainted_Attr *getAttr(const VarDecl *vd);
	Tainted_Attr *getPointerAttr(const VarDecl *vd);
	map<const VarDecl *, Tainted_Attr *>::iterator getmap();
	map<const VarDecl *, Tainted_Attr *>::iterator getend();

	classTmap *getClassTmap(const VarDecl *p);
	void setType(const VarDecl *p, VarDeclType tp);
	void var_attr_set(const VarDecl *p, e_tattr e, const VarDecl *r);
	void var_attr_set(const VarDecl *vd, Tainted_Attr *ta);
	void ptr_set(const VarDecl *p, Tainted_Attr *tp);
	void ptr_attr_set(const VarDecl *p, e_tattr e, const VarDecl *r);
	void classmember_attr_set(const VarDecl *p, classTmap *ct);
	void classmember_attr_set(const VarDecl *p, e_tattr e, const VarDecl *r, Expr *ptrExpr);
	void unionMap(CTmap &b);
	void clear();
	const VarDecl *get_VarDecl(int n);
	bool compareMap(CTmap &tm);
};

//classTmap
class classTmap
{
public:
	classTmap();
	void setCXXRecordDecl(CXXRecordDecl* cxxrd);
	void addMethod(CXXMethodDecl* md);
	void addVar(FieldDecl* fd, VarDeclType evt);
	const std::vector<CXXMethodDecl*>& get_cxxmds();
	const std::vector<FieldDecl*>& get_fds();
	CXXRecordDecl* get_cxxrd()const;
	int getMethodNum();
	int getVarNum();
	CTmap* getMap();

	void classCopy(classTmap* temp);
	void classUnion(classTmap* m, classTmap* a, classTmap* b);
	void classClear();

private:
	CXXRecordDecl* rd;
	std::vector<CXXMethodDecl*> cxxmds;
	std::vector<FieldDecl*> fds;
	CTmap map;
	int methodNum;
	int varNum;
	//int publicVarNum;
	//int privateVarNum;
	//int protectedVarNum;
};



#endif
