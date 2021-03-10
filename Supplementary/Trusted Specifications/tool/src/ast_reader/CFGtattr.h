#ifndef _CFGTATTR_H_
#define _CFGTATTR_H_

class callgraph;
class CTmap;

#include "tmap.h"
#include "AST.h"
#include "callgraph.h"
#include "clang\Analysis\CFG.h"
#include "TaintedStmtAnalysis.h"
#include "Tout.h"

using namespace std;
using namespace clang;
using namespace llvm;

//CFG的输入输出类
class CFGInOut{
private:
	//CFG的输入输出
	CTmap IN, OUT;
public:
	//拷贝构造函数
	CFGInOut()
	{
	}
	CFGInOut(CTmap& b)
	{
		IN.CopyMap(b);
		OUT.CopyMap(b);
	}
	CTmap* GetIN()
	{
		return &IN;
	}
	CTmap* GetOUT()
	{
		return &OUT;
	}
	void setIO(CTmap &tm)
	{
		IN.CopyMap(tm);
		OUT.CopyMap(tm);
	}
};

void checkCFG(clang::CFG &cfg, CTmap &tm, callgraph *cg);
void checkTerminator(CFGBlock &cfgb, CTmap &out, callgraph *cg);

void BuildSecondList(callgraph *caller, callgraph *callee, Tainted_Attr ta[], const int n);
void MsgOutput2Xml(callgraph *cg, Ttable &tt);

void BuildBlockIoTable(map<clang::CFGBlock *, CFGInOut> &block_io_map, clang::CFGBlock *CFGexit, clang::CFGBlock *block, CTmap &tm);
void printBlockMsg(map<clang::CFGBlock *, CFGInOut> &block_io_map, clang::CFGBlock *block);
void printiotable(map<clang::CFGBlock *, CFGInOut> &block_io_map);
void output2xml(callgraph *cg, CTmap &tm);
#endif
