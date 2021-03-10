#include"CFGtattr.h"
#include"AST.h"

//对函数的CFG进行迭代分析
void checkCFG(clang::CFG &cfg, CTmap &tm, callgraph *cg)
{
	cg->set_if_check_cfg();
	clang::CFGBlock* CFGentry = &(cfg.getEntry()), *CFGexit = &(cfg.getExit());
	clang::CFGBlock::pred_iterator pred_it, pred_end;

	CTmap *outm = NULL, *inm = NULL, preout;
	

	BuildBlockIoTable(cg->block_io_map, CFGexit, CFGentry, tm);
	cg->print_cfg();
	//printiotable(block_io_map);

	//主循环，当无OUT发生改变时跳出循环
	int i = 1;
	while (1)
	{
		bool changed = false;

		//为每个block计算其新的out
		cout << "******************************" << endl;
		cout << cg->getCur()->getQualifiedNameAsString() << ": CFG check " << i << " start........." << endl;
		for (map<clang::CFGBlock *, CFGInOut>::reverse_iterator r_iter = cg->block_io_map.rbegin(), r_end = cg->block_io_map.rend(); r_iter != r_end; r_iter++)
		{
			//计算新的in，即对block的前驱的out求并，作为该block的in
			pred_it = r_iter->first->pred_begin(), pred_end = r_iter->first->pred_end();
			clang::CFGBlock* temp = NULL;
			if (r_iter->first != &cfg.getEntry())
			{
				inm = r_iter->second.GetIN();
				inm->clear();
				inm->CopyMap(tm);
				while (pred_it != pred_end)
				{
					temp = pred_it->getReachableBlock();
					//cout << "is B" << temp->getBlockID() << endl;;
					outm = cg->block_io_map[temp].GetOUT();
					inm->unionMap(*outm);
					pred_it++;
				}
			}
			else
			{
				continue;
			}
			outm = r_iter->second.GetOUT();
			preout.CopyMap(*outm);
			outm->CopyMap(*inm);
			
			//checkblock, modify changed
			checkblock(r_iter->first, *outm,cg);
			checkTerminator(*r_iter->first, *outm, cg);

			//if ((*r_iter).first->getTerminatorCondition() == NULL)
			//{
			//	cout << "B" << (*r_iter).first->getBlockID() << " is NULL" << endl;
			//}

			if (outm->compareMap(preout) == false)
			{
				changed = true;
				//printBlockMsg(cg->block_io_map,r_iter->first);
			}
			
		}

		//迭代至所有block的OUT都不发生改变，跳出循环
		cout<<cg->getCur()->getQualifiedNameAsString() << ": CFG check " << i << " end." << endl;
		cout << "******************************" << endl;
		if (changed == false)
			break;
		cg->TCI_list.clear();
		cg->TCI_list_call.clear();
		i++;
	}
	//here to add output
	//printiotable(cg->block_io_map);

	//将函数出口处的tmap填写到callgraph中
	cg->getCTmap().CopyMap(*cg->block_io_map[&cfg.getExit()].GetOUT());
	
	outm = cg->block_io_map[&cfg.getExit()].GetOUT();
	cout << "check " << cg->getCur()->getQualifiedNameAsString() << " end" << endl<<endl;
	//output2xml(cg,*outm);
	
}

void checkTerminator(CFGBlock &cfgb, CTmap &out, callgraph *cg)
{
	unsigned int n = cfgb.getBlockID();
	clang::CFGBlock::pred_iterator pred_it, pred_end;
	pred_it = cfgb.pred_begin();
	pred_end = cfgb.pred_end();
	
	while (pred_it != pred_end)
	{
		if ((pred_it->getReachableBlock()->getBlockID()) <= n)
		{
			const Stmt *stmt = cfgb.getTerminatorCondition();
			checkCond(stmt, out, cg);
		}
		pred_it++;
	}

}

Stmt *checkBeforeCond(CFGBlock &cfgb)
{
	clang::CFGBlock::pred_iterator pred_it, pred_end;
	pred_it = cfgb.pred_begin();
	pred_end = cfgb.pred_end();
	
	Stmt *stmt;

	while (pred_it != pred_end)
	{
		stmt = (*pred_it).getReachableBlock()->getTerminatorCondition();
		if (stmt != NULL)
			return stmt;
		pred_it++;
	}
	return NULL;
}

//ta为参数的污染情况，n为ta数组中元素的个数，该函数用于修改第二个TCI表
void BuildSecondList(callgraph *caller, callgraph *callee, Tainted_Attr ta[], const int n)
{
	TCI *temp;
	Tainted_Attr *p;
	Tainted_Attr *q;
	vector<TCI *>::iterator it, it_end;
	set<const VarDecl *>::iterator var_it, var_end;
	set<const VarDecl *>::iterator var_it2, var_end2;

	int k;

	//g自身的TCI表添加到f中
	it = callee->TCI_list.begin();
	it_end = callee->TCI_list.end();
	//cout << 4444 << endl;
	while (it != it_end)
	{
		//cout << 33333 << endl;
		temp = new TCI;
		temp->astcontext = (*it)->astcontext;
		temp->type = (*it)->type;
		temp->vd = (*it)->vd;
		temp->expr = (*it)->expr;
		temp->re = new Tainted_Attr;
		temp->re->copy((*it)->re);
		temp->fd = (*it)->fd;

	//	temp->re->output();
		//int b;
		//cin >> b;

		caller->TCI_list_call.insert(caller->TCI_list_call.end(),temp);

		it++;
	}

	//g的调用TCI表添加到f中
	it = callee->TCI_list_call.begin();
	it_end = callee->TCI_list_call.end();

	while (it != it_end)
	{
		temp = new TCI;
		temp->astcontext = (*it)->astcontext;
		temp->type = (*it)->type;
		temp->vd = (*it)->vd;
		temp->expr = (*it)->expr;
		temp->re = new Tainted_Attr;
		temp->re->copy((*it)->re);
		temp->fd = (*it)->fd;

		caller->TCI_list_call.insert(caller->TCI_list_call.end(), temp);

		it++;
	}

	//修改f的TCI_list_call中的内容
	it = caller->TCI_list_call.begin();
	it_end = caller->TCI_list_call.end();

	while (it != it_end)
	{
		temp = (*it);
		p = temp->re;
		if (p != NULL)
		{
			var_it = p->getVariableRelation()->begin();
			var_end = p->getVariableRelation()->end();
			while (var_it != var_end)
			{
				k = callee->get_param_no((*var_it));
				//cout<<"testing: " << (*var_it)->getQualifiedNameAsString() << endl;
				if (k >= 0 && k < n)
				{
					q = &ta[k];
					if (q->getVariableAttr() == UNTAINTED)
					{

					}
					//RELATED
					else
					{
						var_it2 = q->getVariableRelation()->begin();
						var_end2 = q->getVariableRelation()->end();
						while (var_it2 != var_end2)
						{
							//将与当今函数的相关信息插入到表中
							p->getVariableRelation()->insert(p->getVariableRelation()->end(), (*var_it2));
							var_it2++;
						}
					}
				}
				var_it++;
			}
		}
		it++;
	}
}

//将需要输出的信息从callgraph放入Ttable供李珺输出
void MsgOutput2Xml(callgraph *cg, Ttable &tt)
{
	SourceManager* sm = NULL;
	Tainted_Attr* ta;

	vector<TCI *>::iterator it, it_end;
	it = cg->TCI_list.begin();
	it_end = cg->TCI_list.end();

	set<const VarDecl *>::iterator var_it, var_end;
	while (it != it_end)
	{
		ta = (*it)->re;
		var_it = ta->getVariableRelation()->begin();
		var_end = ta->getVariableRelation()->end();
		while (var_it != var_end)
		{
			//和main的一个参数相关
			if (cg->get_param_no(*var_it) >= 0)
			{
				sm = &(*it)->astcontext->getSourceManager();
				//cout << (*it)->expr->getLocStart().printToString(*sm) << endl;
				cout << 1 << endl;
				tt.insert((*it)->expr->getLocStart().printToString(*sm), (*it)->fd->getQualifiedNameAsString(), (*it)->type);
				break;
			}
			var_it++;
		}
		it++;
	}

	it = cg->TCI_list_call.begin();
	it_end = cg->TCI_list_call.end();

	while (it != it_end)
	{
		ta = (*it)->re;
		var_it = ta->getVariableRelation()->begin();
		var_end = ta->getVariableRelation()->end();
		while (var_it != var_end)
		{
			//和main的一个参数相关
			if (cg->get_param_no(*var_it) >= 0)
			{
				sm = &(*it)->astcontext->getSourceManager();
				//cout << (*it)->expr->getLocStart().printToString(*sm) << endl;
				cout << 1 << endl;
				tt.insert((*it)->expr->getLocStart().printToString(*sm), (*it)->fd->getQualifiedNameAsString(), (*it)->type);
				break;
			}
			var_it++;
		}
		it++;
	}
}

//为每个语句块创建INOUT污染表
void BuildBlockIoTable(map<clang::CFGBlock *, CFGInOut> &block_io_map, clang::CFGBlock *CFGexit, clang::CFGBlock *block, CTmap &tm)
{
	map<clang::CFGBlock *, CFGInOut>::iterator t = block_io_map.find(block);

	//判断该块是否已添加过
	if (t != block_io_map.end())
		return;
	block_io_map[block].setIO(tm);

	//判断该块是否为出口
	if (block == CFGexit)
		return;

	clang::CFGBlock::succ_iterator it = block->succ_begin(), end = block->succ_end();

	while (it != end)
	{
		//递归创建表
		BuildBlockIoTable(block_io_map, CFGexit, (*it).getReachableBlock(), tm);
		it++;
	}
}

//打印一个block的污染信息
void printBlockMsg(map<clang::CFGBlock *, CFGInOut> &block_io_map, clang::CFGBlock *block)
{
	cout << endl << "------------------------------" << endl;
	cout << "B" << block->getBlockID() << ":" << endl << "IN" << endl;
	block_io_map[block].GetIN()->output();
	cout << endl << "OUT"<<endl;
	block_io_map[block].GetOUT()->output();
	cout << "------------------------------" << endl << endl;
}

//打印当前函数中每个block的污染信息
void printiotable(map<clang::CFGBlock *, CFGInOut> &block_io_map)
{
	cout << endl << "==============================" << endl;
	cout << "Block message print starts...." << endl;
	map<clang::CFGBlock *, CFGInOut>::iterator it = block_io_map.begin(), it_end = block_io_map.end();
	while (it != it_end)
	{
		printBlockMsg(block_io_map, it->first);
		it++;
	}
	cout << "==============================" << endl << endl;
}

//测试用 输出到李珺的表中
void output2xml(callgraph *cg, CTmap &tm)
{
	Ttable tt;
	map<const VarDecl *, Tainted_Attr *>::iterator it = tm.getmap(), it_end = tm.getend();
	cout << 111 << endl;
	while (it != it_end)
	{
		//tt.insert(it->first, 1, cg->getCur()->getQualifiedNameAsString());
		it++;
	}
	cout << 333 << endl;
	//tt.listout();
}
