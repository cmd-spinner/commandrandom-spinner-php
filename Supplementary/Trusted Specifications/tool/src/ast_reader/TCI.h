#ifndef TCI_H
#define TCI_H

#include "AST.h"
#include "tmap.h"

using namespace clang;
using namespace std;
using namespace llvm;

//敏感信息类型
#define TTYPE_ARRAY 1
#define TTYPE_DIV_MOD 2
#define TTYPE_LOOP_BOUND 3
#define TTYPE_MEM 4
#define TTYPE_POINTER_NULL_SET 5
#define TTYPE_RECURSIVE_CALL 6

typedef struct
{
	ASTContext* astcontext;
	int type;
	const VarDecl* vd;
	const Expr* expr;
	Tainted_Attr* re;
	FunctionDecl* fd;
}TCI;

#endif
