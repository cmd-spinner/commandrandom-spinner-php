#ifndef  _TAINTED_STMT_ANALYSIS_
#define _TAINTED_STMT_ANALYSIS_
#include <iostream>
#include <vector>
#include <cstring>

#include "clang/Frontend/ASTUnit.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/FileSystemOptions.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang\Analysis\CFG.h"
#include "callgraph.h"

using namespace clang;
using namespace llvm;


bool checkblock(CFGBlock* cfgb, CTmap &out, callgraph* cg);
bool checkCond(const Stmt* stmt, CTmap &out, callgraph* cg);
int Stmt_analysis(const Stmt* stmt, CTmap &out, callgraph* cg);
Tainted_Attr* Expr_analysis(const Expr* expr, CTmap &out, callgraph* cg);
Tainted_Attr* BinaryOperator_Expr_analysis(const Expr* expr, CTmap &out, callgraph* cg);
Tainted_Attr* CompoundAssignOperator_Expr_analysis(const Expr* expr, CTmap &out, callgraph* cg);
Tainted_Attr* CallExpr_analysis(const Expr* expr, CTmap &out, callgraph* cg);
Tainted_Attr* ArrayExpr_analysis(const Expr* expr, CTmap &out, callgraph* cg);

#endif
