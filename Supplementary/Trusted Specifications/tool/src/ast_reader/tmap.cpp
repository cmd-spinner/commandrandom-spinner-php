#include "tmap.h"

//默认构造函数，污染属性的类型对应为unknown
Tainted_Attr::Tainted_Attr(){
	ptrAttr = NULL;
	is_temp = false;
#ifdef USECLASS
	ptrClassDecl = NULL;
#endif

	type = TYPE_UNKNOWN;
}

//追加了类型参数的构造函数，如果是类请传入该类的classTmap
Tainted_Attr::Tainted_Attr(VarDeclType mytype, classTmap *ct)
{
	type = mytype;
	if (mytype == TYPE_VARIABLE)
	{
		attr = UNTAINTED;
	}
	else if (mytype == TYPE_POINTER)
	{
		attr = UNTAINTED;
		ptrAttr = NULL;
		ptrClassDecl = NULL;
		is_temp = false;
	}
	else if (mytype == TYPE_CLASS)
	{
		ptrClassDecl = ct;
	}
	else
	{
		ptrAttr = NULL;
		ptrClassDecl = NULL;
		type = TYPE_UNKNOWN;
	}
}

//拷贝构造函数
Tainted_Attr::Tainted_Attr(Tainted_Attr& b)
{
	set<const VarDecl *>::iterator it = b.relation.begin(), it_end = b.relation.end();
	type = b.type;
	switch (b.type)
	{
	case TYPE_VARIABLE:
		attr = b.attr;
		while (it != it_end)
		{
			relation.insert(relation.end(), *it);
			it++;
		}
		break;
	case TYPE_CLASS:
		ptrClassDecl = b.ptrClassDecl;
		break;
	case TYPE_POINTER:
		is_temp = b.is_temp;
		ptrAttr = b.ptrAttr;
		while (it != it_end)
		{
			relation.insert(relation.end(), *it);
			it++;
		}
		break;
	case TYPE_UNKNOWN:
		break;
	}
}

Tainted_Attr::~Tainted_Attr()
{
	relation.clear();
	if (type == TYPE_POINTER)
	{
		if (is_temp)
		{
			ptrAttr->~Tainted_Attr();
			delete ptrAttr;
		}
		ptrAttr = NULL;
		type = TYPE_UNKNOWN;
		attr = UNTAINTED;

	}
	if (type == TYPE_CLASS)
	{
		ptrClassDecl->~classTmap();
		delete ptrClassDecl;
		ptrClassDecl = NULL;
	}
}

//获取所存储的污染属性的类型
VarDeclType Tainted_Attr::getType()
{
	return type;
}

//获取变量类型的污染属性的污染情况
e_tattr Tainted_Attr::getVariableAttr()
{
	return attr;
}

//获取变量类型的污染属性的关联
set<const VarDecl *> *Tainted_Attr::getVariableRelation()
{
	return &relation;
}

//获取类类型的污染属性的classTmap指针
classTmap *Tainted_Attr::getClassDecl()
{
	return ptrClassDecl;
}

//获得指针类型的污染属性所指向的污染属性
Tainted_Attr *Tainted_Attr::getPointerAttr()
{
	return ptrAttr;
}

bool Tainted_Attr::getistemp()
{
	return is_temp;
}

//信息输出函数 调试用
void Tainted_Attr::output()
{
	const VarDecl *vd;
	set<const VarDecl *>::iterator it = relation.begin(), it_end = relation.end();
	if (type == TYPE_VARIABLE)
	{
		if (attr == TAINTED)
			cout << "TAINTED ";
		else if (attr == UNTAINTED)
			cout << "UN ";
		else
		{
			cout << "RE Related to: ";
			while (it != it_end)
			{
				vd = *it;
				cout << vd->getNameAsString() << " ";
				it++;
			}
		}
	}
	else if (type == TYPE_POINTER)
	{
		if (attr == TAINTED)
			cout << "TAINTED ";
		else if (attr == UNTAINTED)
			cout << "UN ";
		else
		{
			cout << "RE Related to: ";
			while (it != it_end)
			{
				vd = *it;
				cout << vd->getNameAsString() << " ";
				it++;
			}
		}
	}
	//here to add output
	else
	{
		cout << "unknown";
	}
}


//复制p中的污染属性
void Tainted_Attr::copy(Tainted_Attr *p)
{
	set<const VarDecl *>::iterator it = p->relation.begin(), it_end = p->relation.end();
	if (type == TYPE_CLASS)
	{
		ptrClassDecl->~classTmap();
		delete ptrClassDecl;
	}
	relation.clear();
	type = p->type;
	if (type == TYPE_VARIABLE)
	{
		attr = p->attr;
		if (attr == RELATED)
		{
			while (it != it_end)
			{
				relation.insert(relation.end(), *it);
				it++;
			}
		}
	}
	else if (type == TYPE_POINTER)
	{
		ptrAttr = p->ptrAttr;
		attr = p->attr;
		if (attr == RELATED)
		{
			while (it != it_end)
			{
				relation.insert(relation.end(), *it);
				it++;
			}
		}
		is_temp = p->is_temp;
	}
	else if (type == TYPE_CLASS)
	{
		ptrClassDecl = new classTmap(*p->ptrClassDecl);
	}
}

void Tainted_Attr::settemp(bool b)
{
	if (is_temp == true && b == false)
	{
		ptrAttr->~Tainted_Attr();
		ptrAttr = NULL;
	}
	is_temp = b;
}

//信息设置函数，如果当前污染属性的类型不为VARIABLE，不会进行修改，并警告
void Tainted_Attr::var_attr_set(e_tattr a, const VarDecl *vd)
{
	relation.clear();
	if (type != TYPE_VARIABLE && type != TYPE_POINTER)
	{
		cout << "warning: type != TYPE_VARIABLE" << endl;
		return;
	}

	attr = a;
	if (a == RELATED)
		relation.insert(relation.end(), vd);
}

void Tainted_Attr::var_attr_set(e_tattr a, set<const VarDecl *> r)
{
	relation.clear();
	if (a != RELATED)
	{
		attr = a;
		return;
	}
	set<const VarDecl *>::iterator it = r.begin(), it_end = r.end();
	while (it != it_end)
	{
		relation.insert(relation.end(), *it);
		it++;
	}

}

//信息设置函数，如果当前的污染属性的类型不为CLASS，不会进行修改，并警告
void Tainted_Attr::class_attr_set(e_tattr a, const VarDecl *vd, Expr *ptrExp)
{
	if (type != TYPE_CLASS)
	{
		cout << "warning: type != TYPE_CLASS" << endl;
		return;
	}
	//here to add
}

//信息设置函数，如果当前的污染属性的类型不为POINTER，不会进行修改，并警告
void Tainted_Attr::pointer_attr_set(e_tattr a, const VarDecl *vd)
{

	if (type != TYPE_POINTER)
	{
		cout << "warning: type != TYPE_POINTER" << endl;
		return;
	}
	ptrAttr->var_attr_set(a, vd);
}

//将已生成好的类成员自身的表链接到属性上
void Tainted_Attr::classmember_set(classTmap *ct)
{
	if (ptrClassDecl != NULL)
	{
		ptrClassDecl->~classTmap();
		delete ptrClassDecl;
	}
	ptrClassDecl = ct;
}

//将当前污染属性指向pt指向的位置，如果当前污染属性的类型不为POINTER，不会进行修改，并警告
void Tainted_Attr::setPointer(Tainted_Attr *pt)
{
	if (pt == NULL)
	{
		ptrAttr = NULL;
		return;
	}
	if (type != TYPE_POINTER)
	{
		cout << "Warning: type != POINTER" << endl;
		return;
	}
	while (1)
	{
		if (pt->type == TYPE_VARIABLE || pt->type == TYPE_CLASS)
		{
			copy(pt);
			ptrAttr = pt;
			return;
		}
		pt = pt->ptrAttr;
	}
}

//设置污染属性的类型
void Tainted_Attr::setType(VarDeclType tp)
{
	type = tp;
	if (tp == TYPE_VARIABLE)
	{
		attr = UNTAINTED;
	}
	else if (tp == TYPE_POINTER)
	{
		attr = UNTAINTED;
		ptrAttr = NULL;
		is_temp = false;
	}
	else if (tp == TYPE_CLASS)
	{
		ptrClassDecl = NULL;
	}
	else
	{
		ptrAttr = NULL;
		type = TYPE_UNKNOWN;
	}
}

//将两个污染属性取并，存到当前变量中
void Tainted_Attr::unionAttr(Tainted_Attr &a, Tainted_Attr &b)
{
	set<const VarDecl*>::iterator it, it_end;
	relation.clear();
	if (a.type != b.type)
	{
		cout << "Error in unionAttr()" << endl;
		return;
	}
	type = a.type;
	if (type == TYPE_VARIABLE)
	{
		if (a.attr == TAINTED || b.attr == TAINTED)
		{
			attr = TAINTED;
			return;
		}
		else if (a.attr == UNTAINTED && b.attr == UNTAINTED)
		{
			attr = UNTAINTED;
			return;
		}
		else if (a.attr == RELATED && b.attr != RELATED)
		{
			attr = RELATED;
			copy(&a);
			return;
		}
		else if (a.attr != RELATED && b.attr == RELATED)
		{
			attr = RELATED;
			copy(&b);
			return;
		}
		else
		{
			attr = RELATED;
			copy(&a);
			it = b.relation.begin();
			it_end = b.relation.end();
			while (it != it_end)
			{
				relation.insert(relation.end(), *it);
				it++;
			}
		}
		return;
	}
}

//当前变量与其他变量取并，存到当前变量中
void Tainted_Attr::unionAttr(Tainted_Attr &a)
{
	Tainted_Attr b;
	b.copy(this);
	unionAttr(a, b);
}

//构造函数
CTmap::CTmap(){}

//拷贝构造函数
CTmap::CTmap(CTmap& b)
{
	Tainted_Attr *t = NULL, *newattr;
	const VarDecl *pdec = NULL;
	map<const VarDecl *, Tainted_Attr *>::iterator it = b.tmap.begin(), it_end = b.tmap.end();

	while (it != it_end)
	{
		pdec = (*it).first;
		t = (*it).second;
		newattr = new Tainted_Attr(*t);
		tmap[pdec] = newattr;
		it++;
	}
}

//析构函数
CTmap::~CTmap()
{
	Tainted_Attr *t;
	classTmap *ct;
	map<const VarDecl *, Tainted_Attr *>::iterator iter = tmap.begin(), iter_end = tmap.end();
	while (iter != iter_end)
	{
		t = iter->second;
		if (t->getType() == TYPE_CLASS)
		{
			ct = t->getClassDecl();
			ct->classClear();
			delete ct;
		}
		delete iter->second;
		iter->second = NULL;
		iter++;
	}
	tmap.clear();
}

//map中的元素及对应的污染情况输出
void CTmap::output()
{
	map<const VarDecl *, Tainted_Attr *>::iterator iter = tmap.begin(), iter_end = tmap.end();
	while (iter != iter_end)
	{
		std::cout << iter->first->getQualifiedNameAsString() << " ";
		iter->second->output();
		cout << endl;
		iter++;
	}
}

map<const VarDecl *, Tainted_Attr *>::iterator CTmap::getmap()
{
	return tmap.begin();
}

map<const VarDecl *, Tainted_Attr *>::iterator CTmap::getend()
{
	return tmap.end();
}

//将当前map清空，并将b中的元素及污染属性整个拷贝到map中
void CTmap::CopyMap(CTmap& b)
{
	clear();
	Tainted_Attr *t = NULL, *newattr, *ptrattr;
	const VarDecl *pdec = NULL;
	map<const VarDecl *, Tainted_Attr *>::iterator it = b.tmap.begin(), it_end = b.tmap.end();

	while (it != it_end)
	{
		pdec = it->first;
		t = it->second;
		//pdec==class here to add how to copy

		//若t只是一个普通的变量，则调用一般的复制函数
		if (t->getType() == TYPE_VARIABLE)
		{
			newattr = new Tainted_Attr;
			newattr->copy(t);
			tmap[pdec] = newattr;
		}
		//若为指针
		else if (t->getType() == TYPE_POINTER)
		{
			newattr = new Tainted_Attr;
			newattr->copy(t);
			tmap[pdec] = newattr;
			ptrattr = newattr->getPointerAttr();
			if (ptrattr == NULL)
			{

			}
			else if (newattr->getistemp() == true)
			{
				ptrattr = new Tainted_Attr;
				ptrattr->copy(t->getPointerAttr());
			}
			//是表t中的某个元素
			else
			{
				map<const VarDecl *, Tainted_Attr *>::iterator it2 = b.tmap.begin(), it_end2 = b.tmap.end();
				bool find = false;
				while (it2 != it_end2)
				{
					if (it2->second == ptrattr)
					{
						find = true;
						newattr->setPointer(getAttr(it2->first));
					}
					it2++;
				}
				//是某个类中的元素
			}
		}
		it++;
	}
}

//若p不在表中，插入一个以p为索引的空条目
void CTmap::insert(const VarDecl *p)
{
	Tainted_Attr *t = new Tainted_Attr();

	int count;
	count = tmap.count(p);
	if (count == 0)
	{
		tmap[p] = t;
		//==class here to add how to insert
	}
	else
		delete t;
}

//从map中删除p
void CTmap::del(const VarDecl *p)
{
	Tainted_Attr *t = tmap[p];
	//==class here to add how to delete
	if (t->getType() == TYPE_CLASS)
	{
		t->getClassDecl()->~classTmap();
		delete t->getClassDecl();
	}
	delete t;
	tmap.erase(p);
}

//取得变量定义节点p对应的污染属性
Tainted_Attr *CTmap::getAttr(const VarDecl *p)
{
	int count;
	count = tmap.count(p);
	if (count == 0)
		return NULL;
	else
	{
		return tmap[p];
	}
}

//取得变量定义节点p指针指向的污染属性
Tainted_Attr *CTmap::getPointerAttr(const VarDecl *vd)
{
	int count;
	count = tmap.count(vd);
	if (count == 0)
		return NULL;
	else
	{
		if (tmap[vd]->getType() != TYPE_POINTER)
			return NULL;
		else
			return tmap[vd]->getPointerAttr();
	}
}

//获取类的变量的自身的map
classTmap *CTmap::getClassTmap(const VarDecl *p)
{
	int count;
	count = tmap.count(p);
	if (count == 0)
	{
		return NULL;
	}
	else
	{
		if (tmap[p]->getType() != TYPE_CLASS)
		{
			return NULL;
		}
		return tmap[p]->getClassDecl();
	}
}

//设置某个变量的属性（变量、指针、类）
void CTmap::setType(const VarDecl *p, VarDeclType tp)
{
	int count;
	count = tmap.count(p);
	if (count == 0)
	{
		cout << "Error: No such variable in the function" << endl;
		return;
	}
	else
	{
		tmap[p]->setType(tp);
	}
}

//设置p的污染属性，p为普通变量类型
void CTmap::var_attr_set(const VarDecl *p, e_tattr e, const VarDecl *vd)
{
	//cout << "relation = " << r << endl;
	int count;
	Tainted_Attr *tp;
	count = tmap.count(p);
	if (count == 0)
	{
		cout << "Error: No such variable in the function" << endl;
		return;
	}
	else
	{
		tp = tmap[p];
		if (tp->getType() != TYPE_VARIABLE && tp->getType() != TYPE_POINTER)
		{
			cout << "Warning: type != TYPE_VARIABLE" << endl;
			return;
		}
		tp->var_attr_set(e, vd);
	}
}

//设置vd的污染属性
void CTmap::var_attr_set(const VarDecl *vd, Tainted_Attr *ta)
{
	getAttr(vd)->copy(ta);
}

//设置pt指向的变量
void CTmap::ptr_set(const VarDecl *p, Tainted_Attr *tp)
{
	int count;
	count = tmap.count(p);
	if (count == 0)
	{
		cout << "Error: No such variable in the function" << endl;
		return;
	}
	else
	{
		if (tmap[p]->getType() != TYPE_POINTER)
		{
			cout << "Warning: type != POINTER" << endl;
			return;
		}
		tmap[p]->setPointer(tp);
	}
}

//设置pt指向的变量的污染属性
void CTmap::ptr_attr_set(const VarDecl *p, e_tattr e, const VarDecl *vd)
{
	int count;
	Tainted_Attr *tp;
	count = tmap.count(p);
	if (count == 0)
	{
		cout << "Error: No such variable in the function" << endl;
		return;
	}
	else
	{
		tp = tmap[p];
		if (tp->getType() != TYPE_POINTER)
		{
			cout << "Warning: type != TYPE_POINTER" << endl;
			return;
		}
		tp->pointer_attr_set(e, vd);
	}
}

//将classTmap的指针链接到VarDecl的条目上
void CTmap::classmember_attr_set(const VarDecl *p, classTmap *ct)
{
	int count;
	Tainted_Attr *tp;
	count = tmap.count(p);
	if (count == 0)
	{
		cout << "Error: No such variable in the function" << endl;
		return;
	}
	else
	{
		tp = tmap[p];
		if (tp->getType() != TYPE_CLASS)
		{
			cout << "Warning: type != TYPE_POINTER" << endl;
			return;
		}
		else
		{
			tp->classmember_set(ct);
		}
	}
}

//设置污染属性
void CTmap::classmember_attr_set(const VarDecl *p, e_tattr e, const VarDecl *r, Expr *ptrExpr)
{}

//将两个map中的污染属性合并
void CTmap::unionMap(CTmap &b)
{
	const VarDecl *p;
	map<const VarDecl *, Tainted_Attr *>::iterator iter = tmap.begin(), iter_end = tmap.end();
	while (iter != iter_end)
	{
		p = (*iter).first;
		if (b.getAttr(p) != NULL)
			(*iter).second->unionAttr(*b.getAttr(p));
		iter++;
	}
}

//清空map中的元素
void CTmap::clear()
{
	Tainted_Attr *t;
	classTmap *ct;
	map<const VarDecl *, Tainted_Attr *>::iterator iter = tmap.begin(), iter_end = tmap.end();
	while (iter != iter_end)
	{
		t = iter->second;
		if (t->getType() == TYPE_CLASS)
		{
			ct = t->getClassDecl();
			ct->classClear();
		}
		delete iter->second;
		iter->second = NULL;
		iter++;
	}
	tmap.clear();
}

//获取第n个变量定义
const VarDecl *CTmap::get_VarDecl(int n)
{
	int i = 0;
	map<const VarDecl *, Tainted_Attr *>::iterator iter = tmap.begin(), iter_end = tmap.end();
	for (; iter != iter_end; iter++)
	{
		if (i == n)
			return iter->first;
		i++;
	}
	return NULL;
}

//比较两个属性是否相同，自用
bool Tainted_Attr::compareAttr(Tainted_Attr &ta)
{
	if (type != ta.type)
		return false;
	if (type == TYPE_VARIABLE || type == TYPE_POINTER)
	{
		if (attr != ta.attr)
			return false;
		if (attr == RELATED)
		{
			set<const VarDecl *>::iterator it = relation.begin(), it_end = relation.end();
			while (it != it_end)
			{
				if (ta.relation.find(*it) == ta.relation.end())
					return false;
				it++;
			}
			it = ta.relation.begin();
			it_end = ta.relation.end();
			while (it != it_end)
			{
				if (relation.find(*it) == relation.end())
					return false;
				it++;
			}
		}
	}
	else
	{
		cout << "must Add something" << endl;
	}
	return true;
}

//比较两个map是否相同，相同则返回true，自用
bool CTmap::compareMap(CTmap &tm)
{
	Tainted_Attr *ta;
	map<const VarDecl*, Tainted_Attr *>::iterator it = tmap.begin(), it_end = tmap.end();
	while (it != it_end)
	{
		ta = tm.getAttr(it->first);
		if (ta != NULL)
		{
			if (it->second->compareAttr(*ta) != true)
				return false;
		}
		it++;
	}
	return true;
}

/*void f()
{
Tainted_Attr *ptr_p, *ptr_a;//分别存了p和a的污染状况
//p = &a;(p为指针，a为一般变量)
ptr_p->~Tainted_Attr();
ptr_p->setType(TYPE_POINTER);
ptr_p->setPointer(ptr_a);	//会自动将p的污染属性设置为与a相同，且指针会指向a的条目
//p = p + 1;(指针指向了下一个位置)
if (ptr_p->getistemp() == false)//不是动态创建的变量
ptr_p->setPointer(NULL);//将指针从本来指向的内存移开
else
ptr_p->settemp(false);
//a = (*p) + b;(a，b为变量，p为指针)
ptr_p->getPointerAttr(); //取得指向的变量的污染条目，之后用这个条目与b的取并
//*p = a + b;
ptr_p->getPointerAttr(); //取得指向的变量的污染条目，之后操作与一般变量相同
//p = new int;(例如p指向了动态变量)
Tainted_Attr *temp = new Tainted_Attr();
ptr_p->~Tainted_Attr();
ptr_p->setType(TYPE_POINTER);
ptr_p->settemp(true);
}*/



//classTmap.cpp
classTmap::classTmap()
{
	methodNum = 0;
	varNum = 0;
	rd = NULL;
}
void classTmap::setCXXRecordDecl(CXXRecordDecl* cxxrd)
{
	rd = cxxrd;
}
void classTmap::addMethod(CXXMethodDecl* md)
{
	cxxmds.push_back(md);
	methodNum++;
}
void classTmap::addVar(FieldDecl* fd, VarDeclType evt)
{
	fds.push_back(fd);
	map.insert((VarDecl*)fd);
	map.setType((VarDecl*)fd, evt);
	CXXRecordDecl* t;
	classTmap* ct;
	classTmap* new_ct;
	switch (evt)
	{
	case TYPE_POINTER:break;
	case TYPE_VARIABLE:
		map.var_attr_set((VarDecl*)fd, UNTAINTED, NULL);
		break;
	case TYPE_CLASS:
		t = fd->getType()->getAsCXXRecordDecl();
		ct = getClassTmap(t);
		new_ct = new classTmap();
		new_ct->classCopy(ct);
		map.classmember_attr_set((VarDecl*)fd, new_ct);
		break;
	default:break;
	}
	varNum++;
}
const vector<CXXMethodDecl*>& classTmap::get_cxxmds()
{
	return cxxmds;
}
const vector<FieldDecl*>& classTmap::get_fds()
{
	return fds;
}
CXXRecordDecl* classTmap::get_cxxrd()const
{
	return rd;
}
int classTmap::getMethodNum()
{
	return methodNum;
}
int classTmap::getVarNum()
{
	return varNum;
}

void printClassTmap(vector<classTmap*> CT)
{
	vector<classTmap*>::iterator it_ct = CT.begin(), it_ct_end = CT.end();
	vector<CXXMethodDecl*>::iterator it_cxxmd;
	vector<CXXMethodDecl*> cxxmds;
	int methodNum;
	vector<FieldDecl*>::iterator it_fd;
	vector<FieldDecl*>fds;
	int varNum;
	for (; it_ct != it_ct_end; it_ct++)
	{
		cxxmds = (*it_ct)->get_cxxmds();
		fds = (*it_ct)->get_fds();
		cout << (*it_ct)->get_cxxrd()->getQualifiedNameAsString() << "\n";
		it_cxxmd = cxxmds.begin();
		methodNum = (*it_ct)->getMethodNum();
		it_fd = fds.begin();
		varNum = (*it_ct)->getVarNum();
		cout << "\tVar:\n";
		if (fds.size() > 0)
		{
			for (int i = 0; i < varNum; i++)
			{
				cout << "\t\t" << (*it_fd)->getQualifiedNameAsString() << "\n";
				it_fd++;
			}
		}
		if (cxxmds.size() > 0)
		{
			cout << "\tMethod:\n";
			for (int i = 0; i < methodNum; i++)
			{
				cout << "\t\t" << (*it_cxxmd)->getQualifiedNameAsString() << "\n";
				it_cxxmd++;
			}
		}
	}
}

bool if_find_class(vector<classTmap*>ClassTmap, CXXRecordDecl* rd)
{
	if (ClassTmap.size() == 0)
		return false;
	vector<classTmap*>::iterator it = ClassTmap.begin(), it_end = ClassTmap.end();
	for (; it != it_end; it++)
	{
		if ((*it)->get_cxxrd() == rd)
			return true;
	}
	return false;
}

CTmap* classTmap::getMap()
{
	return &map;
}

void classTmap::classCopy(classTmap* temp)
{
	rd = temp->rd;

	cxxmds.clear();
	vector<CXXMethodDecl*>::iterator it_cxxmd = temp->cxxmds.begin(),
		it_cxxmd_end = temp->cxxmds.end();
	for (; it_cxxmd != it_cxxmd_end; it_cxxmd++)
	{
		cxxmds.push_back((*it_cxxmd));
	}

	fds.clear();
	vector<FieldDecl*>::iterator it_fd = temp->fds.begin(),
		it_fd_end = temp->fds.end();
	for (; it_fd != it_fd_end; it_fd++)
	{
		fds.push_back((*it_fd));
	}

	map.CopyMap(temp->map);

	methodNum = temp->methodNum;
	varNum = temp->varNum;

}

void classTmap::classUnion(classTmap* m, classTmap* a, classTmap* b)
{
	if (a->rd != b->rd)
		cout << "不允许不同的classTmap的合并" << endl;

	//classTmap map_re;
	m->classCopy(a);
	m->map.unionMap(b->map);
}

void classTmap::classClear()
{
	cxxmds.clear();
	fds.clear();
	map.clear();
}

classTmap* getClassTmap(CXXRecordDecl* rd)
{
	int size = ClassTmap.size();
	for (int i = 0; i < size; i++)
	{
		if (ClassTmap[i]->get_cxxrd() == rd)
			return ClassTmap[i];
	}
	return NULL;
}
