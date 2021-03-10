#include "TaintedStmtAnalysis.h"

//Tainted_Attr *StmtTa = new Tainted_Attr();   //each  statement only has one common StmtTa 

/*
interface function ,used to scan CFG Blocks each a time
referenced by
void checkCFG(clang::CFG &cfg, CTmap &tm, callgraph *cg)
defined in "CFGtattr.cpp"
*/

extern vector<callgraph*> Callgraph;
CFGBlock *CFGB;

bool checkblock(CFGBlock* cfgb, CTmap &out, callgraph* cg)
{
	int i = 0;
	CFGB = cfgb;
	CFGBlock::iterator v = cfgb->begin();
	for (; v != cfgb->end(); v++)
	{
		i++;
		//scan each statement
		if (v->getKind() == CFGElement::Kind::Statement)
		{
			CFGStmt s = v->castAs<CFGStmt>();
			Stmt_analysis(s.getStmt(), out, cg);
			//out.output();
		}
		cout << "Stmt " << i << " has been analysised" << endl;
	}
	return false;
}

// 循环上界检测
bool checkCond(const Stmt* stmt, CTmap &out, callgraph* cg)
{
	if (stmt->getStmtClass() == Stmt::BinaryOperatorClass)
	{
		Tainted_Attr* lta, *rta;
		const BinaryOperator* bo = dyn_cast<BinaryOperator>(stmt);
		lta = Expr_analysis(bo->getLHS(), out, cg);
		rta = Expr_analysis(bo->getRHS(), out, cg);
		if (lta->getVariableAttr() == RELATED || rta->getVariableAttr() == RELATED)
		{
			//Error kind = 3
			TCI * tci = new TCI;
			tci->fd = cg->getCur();
			tci->re = new Tainted_Attr;
			tci->re->setType(TYPE_VARIABLE);
			tci->re->unionAttr(*lta, *rta);
			tci->type = 3;
			tci->vd = NULL;
			tci->expr = bo;
			tci->astcontext = cg->getASTContext();
			cg->TCI_list.insert(cg->TCI_list.end(), tci);
		}
	}
	return true;
}
/*
Analysis a statement and modify the outmap at the same time
*/
int Stmt_analysis(const Stmt* stmt, CTmap &out, callgraph* cg)
{
	const DeclStmt* ds;
	const Decl* decl;
	const VarDecl* vd,*vd2;
	const Expr* expr;
	const UnaryOperator* uo;
	DeclGroupRef::const_iterator di;
	//	const ReturnStmt* rs;

	/*
	analysis Stmt in different ways accroding to it's StmtClass
	*/
	switch (stmt->getStmtClass()) {
	case Stmt::CompoundStmtClass:break;
	case Stmt::WhileStmtClass:
		cout << "Get While Stmt Here" << endl;
		break;
	case Stmt::BinaryOperatorClass:
		expr = dyn_cast<Expr>(stmt);
		Expr_analysis(expr, out, cg);
		break;

    //定义语句
	case Stmt::DeclStmtClass:
		ds = dyn_cast<DeclStmt>(stmt);
		if (ds->isSingleDecl())
		{
			decl = ds->getSingleDecl();
			vd = dyn_cast<VarDecl>(decl);
			expr = vd->getInit();
			if (expr != NULL)
			{
				if (out.getAttr(vd)->getType() == TYPE_POINTER)          //指针定义
				{
					if (expr->getStmtClass() == Stmt::UnaryOperatorClass)
					{
						uo = dyn_cast<UnaryOperator>(expr);
						if (uo->getOpcode() == UnaryOperatorKind::UO_AddrOf)    //int *p = &a; 
						{
							Tainted_Attr* ptr_p = out.getAttr(vd);
							//ptr_p->~Tainted_Attr();
							ptr_p->setType(TYPE_POINTER);
							ptr_p->setPointer(Expr_analysis(uo->getSubExpr(),out,cg));
						}
					}
					else if (expr->getStmtClass() == Stmt::ImplicitCastExprClass)
					{
						vd2 = dyn_cast<VarDecl>(dyn_cast<DeclRefExpr>(dyn_cast<ImplicitCastExpr>(expr)->getSubExpr())->getDecl());
						out.getAttr(vd)->setPointer(out.getPointerAttr(vd2));
					}
					else
					{
						Tainted_Attr* ptr_p = out.getAttr(vd);
						//ptr_p->~Tainted_Attr();
						ptr_p = Expr_analysis(expr, out, cg);
						ptr_p->setType(TYPE_POINTER);
					}
				}
				else if (out.getAttr(vd)->getType() == TYPE_VARIABLE)    //变量定义
				{
					out.var_attr_set(vd, Expr_analysis(expr, out, cg));
				}
			}
		}
		else
		{
			di = ds->getDeclGroup().begin();
			for (; di != ds->getDeclGroup().end(); di++)
			{
				vd = dyn_cast<VarDecl>(*di);
				expr = vd->getInit();
				if (expr != NULL)
				{
					if (out.getAttr(vd)->getType() == TYPE_POINTER)          //指针定义
					{
						if (expr->getStmtClass() == Stmt::UnaryOperatorClass)
						{
							uo = dyn_cast<UnaryOperator>(expr);
							if (uo->getOpcode() == UnaryOperatorKind::UO_AddrOf)    //int *p = &a; 
							{
								Tainted_Attr* ptr_p = out.getAttr(vd);
								//ptr_p->~Tainted_Attr();
								ptr_p->setType(TYPE_POINTER);
								ptr_p->setPointer(Expr_analysis(uo->getSubExpr(), out, cg));
								//out.var_attr_set(vd, ptr_p);
							}
						}
						else if (expr->getStmtClass() == Stmt::ImplicitCastExprClass)
						{
							vd2 = dyn_cast<VarDecl>(dyn_cast<DeclRefExpr>(dyn_cast<ImplicitCastExpr>(expr)->getSubExpr())->getDecl());
							out.getAttr(vd)->setPointer(out.getPointerAttr(vd2));
						}
						else
						{
							Tainted_Attr* ptr_p = out.getAttr(vd);
							//ptr_p->~Tainted_Attr();
							ptr_p = Expr_analysis(expr, out, cg);
							ptr_p->setType(TYPE_POINTER);
						}
					}
					else if (out.getAttr(vd)->getType() == TYPE_VARIABLE)    //变量定义
					{
						out.var_attr_set(vd, Expr_analysis(expr, out, cg));
					}
				}
			}
		}
		break;
	case Stmt::ReturnStmtClass:
		expr = dyn_cast<ReturnStmt>(stmt)->getRetValue();
		//get tainted_attr of the return expr
		if (expr != NULL)
		{
			cg->get_return()->unionAttr(*Expr_analysis(expr, out, cg));
		}
		break;
	case Stmt::CallExprClass:
		expr = dyn_cast <Expr>(stmt);
		Expr_analysis(expr, out, cg);
		break;
	case Stmt::CompoundAssignOperatorClass:
		expr = dyn_cast <Expr>(stmt);
		Expr_analysis(expr, out, cg);
		break;
	default:break;
	}
	return 0;
}

Tainted_Attr* Expr_analysis(const Expr* expr, CTmap &out, callgraph* cg)
{
	const VarDecl *vd;
	const ImplicitCastExpr* icexpr;
	const DeclRefExpr* drexpr;
	const UnaryOperator* uo;
	const ParenExpr* pexpr;
	Tainted_Attr* temp;
	Tainted_Attr* res = new Tainted_Attr();
	res->setType(TYPE_VARIABLE);
	if (expr == NULL)
	{
		return res;
	}
	switch (expr->getStmtClass()) {
	case Stmt::BinaryOperatorClass:
		return BinaryOperator_Expr_analysis(expr, out, cg);
	case Stmt::BlockExprClass:break;
	case Stmt::CallExprClass:
		return CallExpr_analysis(expr, out, cg);
	case Stmt::ImplicitCastExprClass:
		icexpr = dyn_cast<ImplicitCastExpr>(expr);
		return Expr_analysis(icexpr->getSubExpr(), out, cg);
	case Stmt::DeclRefExprClass:
		drexpr = dyn_cast<DeclRefExpr>(expr);
		vd = dyn_cast<VarDecl>(drexpr->getDecl());
		res->copy(out.getAttr(vd));
		return res;
	case Stmt::IntegerLiteralClass:
		return res;
	case Stmt::UnaryOperatorClass:
		uo = dyn_cast<UnaryOperator>(expr);
		if (uo->getOpcode() == UnaryOperatorKind::UO_Deref)
		{
			if (uo->getSubExpr()->getStmtClass() == Stmt::ImplicitCastExprClass)
			{
				icexpr = dyn_cast<ImplicitCastExpr>(uo->getSubExpr());
				if (icexpr->getSubExpr()->getStmtClass() == Stmt::DeclRefExprClass)
				{
					drexpr = dyn_cast<DeclRefExpr>(icexpr->getSubExpr());
					vd = dyn_cast<VarDecl>(drexpr->getDecl());
					if (out.getAttr(vd)->getType() == TYPE_POINTER)
					{
						cout << "::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
						return out.getAttr(vd)->getPointerAttr();
					}
				}
			}
			else if (uo->getSubExpr()->getStmtClass() == Stmt::DeclRefExprClass)
			{
				drexpr = dyn_cast<DeclRefExpr>(uo->getSubExpr());
				vd = dyn_cast<VarDecl>(drexpr->getDecl());
				if (out.getAttr(vd)->getType() == TYPE_POINTER)
				{
					//out.getAttr(vd)->getPointerAttr()->output();
					return out.getAttr(vd)->getPointerAttr();	
				}
			}
			else
			{
				temp = Expr_analysis(uo->getSubExpr(), out, cg);

				//ErrorKind == 7    (取地址操作操作数污染)
				if (temp->getVariableAttr() == RELATED)
				{
					TCI * tci = new TCI;
					tci->fd = cg->getCur();
					tci->re = temp;
					tci->type = 7;
					tci->vd = NULL;
					tci->expr = expr;
					tci->astcontext = cg->getASTContext();
					cg->TCI_list.insert(cg->TCI_list.end(), tci);
				}
			}
			return res;
		}
		else
		{
			return Expr_analysis(uo->getSubExpr(), out, cg);
		}
	case Stmt::ArraySubscriptExprClass:
		return ArrayExpr_analysis(expr, out, cg);
	case Stmt::ParenExprClass:
		pexpr = dyn_cast<ParenExpr>(expr);
		return Expr_analysis(pexpr->getSubExpr(), out, cg);
	case Stmt::CompoundAssignOperatorClass:
		CompoundAssignOperator_Expr_analysis(expr,out,cg);
	default:break;
	}
	return res;
}

Tainted_Attr* BinaryOperator_Expr_analysis(const Expr* expr, CTmap &out, callgraph* cg)
{
	Tainted_Attr* res = new Tainted_Attr();
	res->setType(TYPE_VARIABLE);
	Tainted_Attr* ata = new Tainted_Attr();
	ata->setType(TYPE_VARIABLE);
	const BinaryOperator* bo = dyn_cast<BinaryOperator>(expr);
	const Expr* lexpr = bo->getLHS();
	const Expr* rexpr = bo->getRHS();
	const VarDecl *vd,*lvd,*rvd;
	const DeclRefExpr* drexpr;
	const UnaryOperator* uo;
	const ImplicitCastExpr* icexpr;
	const Expr* arrayBase;
	const Expr* arrayIdx;
	Tainted_Attr* lta, *rta,*temp;
	const ArraySubscriptExpr *asexpr;
	switch (bo->getOpcode()) {
	case BinaryOperatorKind::BO_LAnd:                  //if && is in a Expr other than IfExpr ,there is still some problens
	case BinaryOperatorKind::BO_Add:
	case BinaryOperatorKind::BO_And:
	case BinaryOperatorKind::BO_Div:
	case BinaryOperatorKind::BO_Mul:
	case BinaryOperatorKind::BO_Sub:
	case BinaryOperatorKind::BO_EQ:
	case BinaryOperatorKind::BO_GE:
	case BinaryOperatorKind::BO_GT:
	case BinaryOperatorKind::BO_LE:
	case BinaryOperatorKind::BO_LOr:
	case BinaryOperatorKind::BO_LT:
	case BinaryOperatorKind::BO_NE:
	case BinaryOperatorKind::BO_Or:
	case BinaryOperatorKind::BO_Rem:
	case BinaryOperatorKind::BO_Shl:
	case BinaryOperatorKind::BO_Shr:
	case BinaryOperatorKind::BO_Xor:
		lta = Expr_analysis(lexpr, out, cg);
		rta = Expr_analysis(rexpr, out, cg);

		//除模操作检测
		if (bo->getOpcode() == BinaryOperatorKind::BO_Div || bo->getOpcode() == BinaryOperatorKind::BO_Rem)
		{
			if (rta->getVariableAttr() == RELATED)
			{
				cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
				if (rexpr->getStmtClass() == Stmt::ImplicitCastExprClass)
				{
					cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
					icexpr = dyn_cast<ImplicitCastExpr>(rexpr);
					if (icexpr->getSubExpr()->getStmtClass() == Stmt::DeclRefExprClass)
					{
						drexpr = dyn_cast<DeclRefExpr>(icexpr->getSubExpr());
						vd = dyn_cast<VarDecl>(drexpr->getDecl());
					}
					else if (icexpr->getSubExpr()->getStmtClass() == Stmt::UnaryOperatorClass)
					{
						uo = dyn_cast<UnaryOperator>(icexpr->getSubExpr());
						icexpr = dyn_cast<ImplicitCastExpr>(uo->getSubExpr());
						drexpr = dyn_cast<DeclRefExpr>(icexpr->getSubExpr());
						vd = dyn_cast<VarDecl>(drexpr->getDecl());
					}
					else if (icexpr->getSubExpr()->getStmtClass() == Stmt::ArraySubscriptExprClass)
					{
						asexpr = dyn_cast<ArraySubscriptExpr>(icexpr);
						arrayBase = asexpr->getBase();
						arrayIdx = asexpr->getIdx();
						while (arrayBase->getStmtClass() == Stmt::ArraySubscriptExprClass || arrayBase->getStmtClass() == Stmt::ImplicitCastExprClass)
						{
							if (arrayBase->getStmtClass() == Stmt::ArraySubscriptExprClass)
							{
								asexpr = dyn_cast<ArraySubscriptExpr>(arrayBase);
								arrayBase = asexpr->getBase();
								arrayIdx = asexpr->getIdx();
							}
							else
							{
								arrayBase = dyn_cast<ImplicitCastExpr>(asexpr->getBase())->getSubExpr();
							}
						}
						drexpr = dyn_cast<DeclRefExpr>(arrayBase);
						vd = dyn_cast<VarDecl>(drexpr->getDecl());
					}
					Stmt *cond;
					cond = checkBeforeCond(*CFGB);
					if (cond != NULL)
					{
						if (cond->getStmtClass() == Stmt::BinaryOperatorClass)
						{
							bo = dyn_cast<BinaryOperator>(cond);
							lexpr = bo->getLHS();
							rexpr = bo->getRHS();
							if (lexpr->getStmtClass() == Stmt::ImplicitCastExprClass)
							{
								icexpr = dyn_cast<ImplicitCastExpr>(lexpr);
								if (icexpr->getSubExpr()->getStmtClass() == Stmt::DeclRefExprClass)
								{
									drexpr = dyn_cast<DeclRefExpr>(icexpr->getSubExpr());
									lvd = dyn_cast<VarDecl>(drexpr->getDecl());
								}
								else if (icexpr->getSubExpr()->getStmtClass() == Stmt::UnaryOperatorClass)
								{
									uo = dyn_cast<UnaryOperator>(icexpr->getSubExpr());
									drexpr = dyn_cast<DeclRefExpr>(uo->getSubExpr());
									lvd = dyn_cast<VarDecl>(drexpr->getDecl());
								}
								else if (icexpr->getSubExpr()->getStmtClass() == Stmt::ArraySubscriptExprClass)
								{
									asexpr = dyn_cast<ArraySubscriptExpr>(icexpr);
									arrayBase = asexpr->getBase();
									arrayIdx = asexpr->getIdx();
									while (arrayBase->getStmtClass() == Stmt::ArraySubscriptExprClass || arrayBase->getStmtClass() == Stmt::ImplicitCastExprClass)
									{
										if (arrayBase->getStmtClass() == Stmt::ArraySubscriptExprClass)
										{
											asexpr = dyn_cast<ArraySubscriptExpr>(arrayBase);
											arrayBase = asexpr->getBase();
											arrayIdx = asexpr->getIdx();
										}
										else
										{
											arrayBase = dyn_cast<ImplicitCastExpr>(asexpr->getBase())->getSubExpr();
										}
									}
									drexpr = dyn_cast<DeclRefExpr>(arrayBase);
									lvd = dyn_cast<VarDecl>(drexpr->getDecl());
								}
							}
							if (rexpr->getStmtClass() == Stmt::ImplicitCastExprClass)
							{
								icexpr = dyn_cast<ImplicitCastExpr>(rexpr);
								if (icexpr->getSubExpr()->getStmtClass() == Stmt::DeclRefExprClass)
								{
									drexpr = dyn_cast<DeclRefExpr>(icexpr->getSubExpr());
									rvd = dyn_cast<VarDecl>(drexpr->getDecl());
								}
								else if (icexpr->getSubExpr()->getStmtClass() == Stmt::UnaryOperatorClass)
								{
									uo = dyn_cast<UnaryOperator>(icexpr->getSubExpr());
									drexpr = dyn_cast<DeclRefExpr>(uo->getSubExpr());
									rvd = dyn_cast<VarDecl>(drexpr->getDecl());
								}
								else if (icexpr->getSubExpr()->getStmtClass() == Stmt::ArraySubscriptExprClass)
								{
									asexpr = dyn_cast<ArraySubscriptExpr>(icexpr);
									arrayBase = asexpr->getBase();
									arrayIdx = asexpr->getIdx();
									while (arrayBase->getStmtClass() == Stmt::ArraySubscriptExprClass || arrayBase->getStmtClass() == Stmt::ImplicitCastExprClass)
									{
										if (arrayBase->getStmtClass() == Stmt::ArraySubscriptExprClass)
										{
											asexpr = dyn_cast<ArraySubscriptExpr>(arrayBase);
											arrayBase = asexpr->getBase();
											arrayIdx = asexpr->getIdx();
										}
										else
										{
											arrayBase = dyn_cast<ImplicitCastExpr>(asexpr->getBase())->getSubExpr();
										}
									}
									drexpr = dyn_cast<DeclRefExpr>(arrayBase);
									rvd = dyn_cast<VarDecl>(drexpr->getDecl());
								}
							}
							if (vd == lvd || vd ==rvd)
							{
									res->unionAttr(*lta, *rta);
									delete lta, rta;
									return res;
							}
						}
					}
				}
				TCI * tci = new TCI;
				tci->fd = cg->getCur();
				tci->re = rta;
				tci->type = 2;
				tci->vd = NULL;
				tci->expr = expr;
				tci->astcontext = cg->getASTContext();
				cg->TCI_list.insert(cg->TCI_list.end(), tci);
			}
		}
		res->unionAttr(*lta, *rta);
		delete lta,rta;
		return res;
	case BinaryOperatorKind::BO_AndAssign:
	case BinaryOperatorKind::BO_AddAssign:
	case BinaryOperatorKind::BO_DivAssign:	
	case BinaryOperatorKind::BO_MulAssign:
	case BinaryOperatorKind::BO_SubAssign:
	case BinaryOperatorKind::BO_OrAssign:
	case BinaryOperatorKind::BO_RemAssign:
	case BinaryOperatorKind::BO_ShlAssign:
	case BinaryOperatorKind::BO_ShrAssign:
	case BinaryOperatorKind::BO_XorAssign:
		if (lexpr->getStmtClass() == Stmt::ArraySubscriptExprClass)
		{
			//多维函数，循环操作取数组名称
			asexpr = dyn_cast<ArraySubscriptExpr>(lexpr);
			arrayBase = asexpr->getBase();
			arrayIdx = asexpr->getIdx();
			ata->unionAttr(*Expr_analysis(arrayIdx, out, cg));
			while (arrayBase->getStmtClass() == Stmt::ArraySubscriptExprClass || arrayBase->getStmtClass() == Stmt::ImplicitCastExprClass)
			{
				if (arrayBase->getStmtClass() == Stmt::ArraySubscriptExprClass)
				{
					ata->unionAttr(*Expr_analysis(arrayIdx, out, cg));
					asexpr = dyn_cast<ArraySubscriptExpr>(arrayBase);
					arrayBase = asexpr->getBase();
					arrayIdx = asexpr->getIdx();
				}
				else
				{
					arrayBase = dyn_cast<ImplicitCastExpr>(asexpr->getBase())->getSubExpr();
				}
			}
			drexpr = dyn_cast<DeclRefExpr>(arrayBase);

			//Error kind  = 1 (array tainted)
			if (ata->getVariableAttr() == RELATED)
			{
				TCI * tci = new TCI;
				tci->fd = cg->getCur();
				tci->re = ata;
				tci->type = 1;
				tci->vd = dyn_cast<VarDecl>(drexpr->getDecl());;
				tci->expr = expr;
				tci->astcontext = cg->getASTContext();
				cg->TCI_list.insert(cg->TCI_list.end(), tci);
			}
		}
		else if (lexpr->getStmtClass() == Stmt::UnaryOperatorClass)
		{
			cout << "::::::::::::::::::::::::LLALAL:::::::::::::::::::::::::::::::::" << endl;
			uo = dyn_cast<UnaryOperator>(lexpr);
			if (uo->getOpcode() == UnaryOperatorKind::UO_Deref)           //等号左边为取指针操作
			{
				res = Expr_analysis(rexpr, out, cg);
				if (uo->getSubExpr()->getStmtClass() == Stmt::ImplicitCastExprClass)
				{
					icexpr = dyn_cast<ImplicitCastExpr>(uo->getSubExpr());
					drexpr = dyn_cast<DeclRefExpr>(icexpr->getSubExpr());
					vd = dyn_cast<VarDecl>(drexpr->getDecl());
					if (out.getPointerAttr(vd) != NULL)
						out.getPointerAttr(vd)->unionAttr(*res);
				}
				else
				{
					temp = Expr_analysis(uo->getSubExpr(), out, cg);

					//ErrorKind  == 7
					if (temp->getVariableAttr() == RELATED)
					{
						TCI * tci = new TCI;
						tci->fd = cg->getCur();
						tci->re = temp;
						tci->type = 7;
						tci->vd = NULL;
						tci->expr = expr;
						tci->astcontext = cg->getASTContext();
						cg->TCI_list.insert(cg->TCI_list.end(), tci);
					}
				}
			}
		}
		else
		{
			drexpr = dyn_cast<DeclRefExpr>(lexpr);
		}
		vd = dyn_cast<VarDecl>(drexpr->getDecl());  //获取到最终的VarDecl

		rta = Expr_analysis(rexpr, out, cg);

		//除法操作检查
		if (bo->getOpcode() == BinaryOperatorKind::BO_DivAssign)
		{
			if (rta->getVariableAttr() == RELATED)
			{
				TCI * tci = new TCI;
				tci->fd = cg->getCur();
				tci->re = rta;
				tci->type = 2;
				tci->vd = NULL;
				tci->expr = expr;
				tci->astcontext = cg->getASTContext();
				cg->TCI_list.insert(cg->TCI_list.end(), tci);
			}
		}
		res->unionAttr(*rta);
		out.var_attr_set(vd, res);
		return res;

	//双目操作符为等号
	case BinaryOperatorKind::BO_Assign:  
		if (lexpr->getStmtClass() == Stmt::ArraySubscriptExprClass)
		{
			//多维函数，循环操作取数组名称
			asexpr = dyn_cast<ArraySubscriptExpr>(lexpr);
			arrayBase = asexpr->getBase();
			arrayIdx = asexpr->getIdx();
			ata->unionAttr(*Expr_analysis(arrayIdx, out, cg));
			while (arrayBase->getStmtClass() == Stmt::ArraySubscriptExprClass || arrayBase->getStmtClass() == Stmt::ImplicitCastExprClass)
			{
				if (arrayBase->getStmtClass() == Stmt::ArraySubscriptExprClass)
				{
					asexpr = dyn_cast<ArraySubscriptExpr>(arrayBase);
					arrayBase = asexpr->getBase();
					arrayIdx = asexpr->getIdx();
					ata->unionAttr(*Expr_analysis(arrayIdx, out, cg));
				}
				else
				{
					arrayBase = dyn_cast<ImplicitCastExpr>(asexpr->getBase())->getSubExpr();
				}
			}
			drexpr = dyn_cast<DeclRefExpr>(arrayBase);

			//Error kind  = 1 (array tainted)
			if (ata->getVariableAttr() == RELATED)
			{
				TCI * tci = new TCI;
				tci->fd = cg->getCur();
				tci->re = ata;
				tci->type = 1;
				tci->vd = dyn_cast<VarDecl>(drexpr->getDecl());;
				tci->expr = expr;
				tci->astcontext = cg->getASTContext();
				cg->TCI_list.insert(cg->TCI_list.end(), tci);
			}

			vd = dyn_cast<VarDecl>(drexpr->getDecl());
			res = Expr_analysis(rexpr, out, cg);
			out.var_attr_set(vd, res);

		}

		else if (lexpr->getStmtClass() == Stmt::UnaryOperatorClass)
		{
			uo = dyn_cast<UnaryOperator>(lexpr);
			if (uo->getOpcode() == UnaryOperatorKind::UO_Deref)           //等号左边为取指针操作
			{
				res = Expr_analysis(rexpr, out, cg);
				if (uo->getSubExpr()->getStmtClass() == Stmt::ImplicitCastExprClass)
				{
					icexpr = dyn_cast<ImplicitCastExpr>(uo->getSubExpr());
					drexpr = dyn_cast<DeclRefExpr>(icexpr->getSubExpr());
					vd = dyn_cast<VarDecl>(drexpr->getDecl());
					if (out.getPointerAttr(vd) != NULL)
						out.getPointerAttr(vd)->copy(res);
				}
				else
				{
					temp = Expr_analysis(uo->getSubExpr(), out, cg);

					//ErrorKind  == 7
					if (temp->getVariableAttr() == RELATED)
					{
						TCI * tci = new TCI;
						tci->fd = cg->getCur();
						tci->re = temp;
						tci->type = 7;
						tci->vd = NULL;
						tci->expr = expr;
						tci->astcontext = cg->getASTContext();
						cg->TCI_list.insert(cg->TCI_list.end(), tci);
					}
				}
			}
		}
		else
		{
			drexpr = dyn_cast<DeclRefExpr>(lexpr);
			vd = dyn_cast<VarDecl>(drexpr->getDecl());
			if (rexpr->getStmtClass() == Stmt::UnaryOperatorClass)
			{
				uo = dyn_cast<UnaryOperator>(rexpr);
				if (uo->getOpcode() == UnaryOperatorKind::UO_AddrOf)
				{
					Tainted_Attr* ptr_p = out.getAttr(vd);
					ptr_p->~Tainted_Attr();
					ptr_p->setType(TYPE_POINTER);
					res = Expr_analysis(uo->getSubExpr(), out, cg);
					ptr_p->setPointer(res);
					return res;
				}
			}

			res = Expr_analysis(rexpr, out, cg);
			out.var_attr_set(vd, res);
		}
		return res;
	case BinaryOperatorKind::BO_Comma:
		Expr_analysis(lexpr, out, cg);
		Expr_analysis(rexpr, out, cg);
		return res;
	case BinaryOperatorKind::BO_PtrMemD:break;
	case BinaryOperatorKind::BO_PtrMemI:break;
	default:break;
	}
	return res;
}

Tainted_Attr* CompoundAssignOperator_Expr_analysis(const Expr* expr, CTmap &out, callgraph* cg)
{
	Tainted_Attr* res = new Tainted_Attr();
	res->setType(TYPE_VARIABLE);
	Tainted_Attr* ata = new Tainted_Attr();
	ata->setType(TYPE_VARIABLE);
	const CompoundAssignOperator* cao = dyn_cast<CompoundAssignOperator>(expr);
	const Expr* lexpr = cao->getLHS();
	const Expr* rexpr = cao->getRHS();
	const VarDecl *vd;
	const DeclRefExpr* drexpr;
	const UnaryOperator* uo;
	const ImplicitCastExpr* icexpr;
	const Expr* arrayBase;
	const Expr* arrayIdx;
	Tainted_Attr* lta, *rta, *temp;
	const ArraySubscriptExpr *asexpr;
	if (lexpr->getStmtClass() == Stmt::ArraySubscriptExprClass)
	{
		//多维函数，循环操作取数组名称
		asexpr = dyn_cast<ArraySubscriptExpr>(lexpr);
		arrayBase = asexpr->getBase();
		arrayIdx = asexpr->getIdx();
		ata->unionAttr(*Expr_analysis(arrayIdx, out, cg));
		while (arrayBase->getStmtClass() == Stmt::ArraySubscriptExprClass || arrayBase->getStmtClass() == Stmt::ImplicitCastExprClass)
		{
			if (arrayBase->getStmtClass() == Stmt::ArraySubscriptExprClass)
			{
				ata->unionAttr(*Expr_analysis(arrayIdx, out, cg));
				asexpr = dyn_cast<ArraySubscriptExpr>(arrayBase);
				arrayBase = asexpr->getBase();
				arrayIdx = asexpr->getIdx();
			}
			else
			{
				arrayBase = dyn_cast<ImplicitCastExpr>(asexpr->getBase())->getSubExpr();
			}
		}
		drexpr = dyn_cast<DeclRefExpr>(arrayBase);

		//Error kind  = 1 (array tainted)
		if (ata->getVariableAttr() == RELATED)
		{
			TCI * tci = new TCI;
			tci->fd = cg->getCur();
			tci->re = ata;
			tci->type = 1;
			tci->vd = dyn_cast<VarDecl>(drexpr->getDecl());;
			tci->expr = expr;
			tci->astcontext = cg->getASTContext();
			cg->TCI_list.insert(cg->TCI_list.end(), tci);
		}
	}
	else if (lexpr->getStmtClass() == Stmt::UnaryOperatorClass)
	{
		cout << "::::::::::::::::::::::::LLALAL:::::::::::::::::::::::::::::::::" << endl;
		uo = dyn_cast<UnaryOperator>(lexpr);
		if (uo->getOpcode() == UnaryOperatorKind::UO_Deref)           //等号左边为取指针操作
		{
			res = Expr_analysis(rexpr, out, cg);
			if (uo->getSubExpr()->getStmtClass() == Stmt::ImplicitCastExprClass)
			{
				icexpr = dyn_cast<ImplicitCastExpr>(uo->getSubExpr());
				drexpr = dyn_cast<DeclRefExpr>(icexpr->getSubExpr());
				vd = dyn_cast<VarDecl>(drexpr->getDecl());
				if (out.getPointerAttr(vd) != NULL)
					out.getPointerAttr(vd)->unionAttr(*res);
			}
			else
			{
				temp = Expr_analysis(uo->getSubExpr(), out, cg);

				//ErrorKind  == 7
				if (temp->getVariableAttr() == RELATED)
				{
					TCI * tci = new TCI;
					tci->fd = cg->getCur();
					tci->re = temp;
					tci->type = 7;
					tci->vd = NULL;
					tci->expr = expr;
					tci->astcontext = cg->getASTContext();
					cg->TCI_list.insert(cg->TCI_list.end(), tci);
				}
			}
		}
	}
	else
	{
		drexpr = dyn_cast<DeclRefExpr>(lexpr);
	}
	vd = dyn_cast<VarDecl>(drexpr->getDecl());  //获取到最终的VarDecl

	rta = Expr_analysis(rexpr, out, cg);

	//除法操作检查
	if (cao->getOpcode() == BinaryOperatorKind::BO_DivAssign)
	{
		if (rta->getVariableAttr() == RELATED)
		{
			TCI * tci = new TCI;
			tci->fd = cg->getCur();
			tci->re = rta;
			tci->type = 2;
			tci->vd = NULL;
			tci->expr = expr;
			tci->astcontext = cg->getASTContext();
			cg->TCI_list.insert(cg->TCI_list.end(), tci);
		}
	}
	res->unionAttr(*rta);
	out.var_attr_set(vd, res);
	return res;
}

Tainted_Attr* CallExpr_analysis(const Expr* expr, CTmap &out, callgraph* cg)
{
	Tainted_Attr* res = new Tainted_Attr();
	res->setType(TYPE_VARIABLE);
	const CallExpr* cexpr;
	const FunctionDecl* fd;
	callgraph* calleeCg;
	cexpr = dyn_cast<CallExpr>(expr);
	fd = cexpr->getDirectCallee();
	calleeCg = findById(Callgraph, fd->getQualifiedNameAsString());
	if (calleeCg == NULL)    //the function can't be found ,which means its in the lib
	{
		/*
		if (strcmp(fd->getQualifiedNameAsString, "free")
		{
		}
		if (cexpr->getNumArgs() == 1)
		{
		cout << "11111111111111111111111111111111111111111111111111111111" << endl;
		//return Expr_analysis(cexpr->getArg(),out,cg);
		}
		else if (cexpr->getNumArgs() > 1)
		{
		cout << "22222222222222222222222222222222222" << endl;
		}
		*/
	}
	else
	{
		/*
		check if the funtion has been analysised
		*/
		//cout << cg->is_callee(fd) << ":::::::::::::::::::::::::::::::::::::::" << endl;
		if (cg->is_callee(fd))
		{
			if (calleeCg->get_if_check_cfg() == false)
			{
				checkCFG(*(calleeCg->get_cfg()), calleeCg->getCTmap(), calleeCg);
				calleeCg->set_if_check_cfg();
			}
			int n = calleeCg->getParamNum();
			Tainted_Attr *tp = new Tainted_Attr[n];

			set<const VarDecl*>::iterator it = calleeCg->get_return()->getVariableRelation()->begin(), it_end = calleeCg->get_return()->getVariableRelation()->end();
			int argNo;
			int i = 0;
			while (it != it_end)
			{
				argNo = calleeCg->get_param_no(*it);
				if (argNo == -1)
				{
					cout << "argNo == -1 which means it can't be found" << endl;
				}
				else
				{
					tp[i].copy(Expr_analysis(cexpr->getArg(argNo), out, cg));
					//tp[i].output();
					//cout << endl;
					res->unionAttr(tp[i]);     //将形参序号转化为实参的具体Expr
					//res->output();
					//cout << endl;
				}
				i++;
				it++;
			}
			BuildSecondList(cg, calleeCg, tp, n);
			delete[] tp;
		}
		else
		{
			return res;
		}
	}
	return res;
}

Tainted_Attr* ArrayExpr_analysis(const Expr* expr, CTmap &out, callgraph* cg)
{
	Tainted_Attr* res = new Tainted_Attr();
	res->setType(TYPE_VARIABLE);
	const ArraySubscriptExpr *asexpr;
	const Expr* arrayBase;
	const Expr* arrayIdx;
	const VarDecl* vd;
	const ImplicitCastExpr* icexpr;
	const DeclRefExpr* drexpr;
	asexpr = dyn_cast<ArraySubscriptExpr>(expr);
	arrayBase = asexpr->getBase();
	arrayIdx = asexpr->getIdx();
	while (arrayBase->getStmtClass() == Stmt::ArraySubscriptExprClass || arrayBase->getStmtClass() == Stmt::ImplicitCastExprClass)
	{
		if (arrayBase->getStmtClass() == Stmt::ArraySubscriptExprClass)
		{
			res->unionAttr(*Expr_analysis(arrayIdx, out, cg));
			asexpr = dyn_cast<ArraySubscriptExpr>(arrayBase);
			arrayBase = asexpr->getBase();
			arrayIdx = asexpr->getIdx();
		}
		else
		{
			arrayBase = dyn_cast<ImplicitCastExpr>(asexpr->getBase())->getSubExpr();
		}
	}
	res->unionAttr(*Expr_analysis(arrayIdx, out, cg));
	if (arrayBase->getStmtClass() == Stmt::ImplicitCastExprClass)
	{
		icexpr = dyn_cast<ImplicitCastExpr>(arrayBase);
		arrayBase = icexpr->getSubExpr();
	}
	if (arrayBase->getStmtClass() == Stmt::DeclRefExprClass)
	{
		drexpr = dyn_cast<DeclRefExpr>(expr);
		vd = dyn_cast<VarDecl>(drexpr->getDecl());

		//Error kind  = 1 (array tainted)
		if (res->getVariableAttr() == RELATED)
		{
			TCI * tci = new TCI;
			tci->fd = cg->getCur();
			tci->re = res;
			tci->type = 1;
			tci->vd = vd;
			tci->expr = expr;
			tci->astcontext = cg->getASTContext();
			cg->TCI_list.insert(cg->TCI_list.end(), tci);
		}
		out.var_attr_set(vd, res);
	}
	else         //exception
	{
		cout << "ArrayBase is not a Decl or ImplicateCast" << endl;
	}
	return res;
}
