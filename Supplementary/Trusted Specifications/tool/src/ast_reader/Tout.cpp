#include"Tout.h"
Ttable::Ttable()
{
	head = NULL;

}
Ttable::~Ttable()
{
	Node*p = NULL;
	while (p == head)
	{
		p = head;
		while (p != NULL)
		{

			p = p->next;
		}
		delete p;
	}
}

bool Ttable::CreateXmlFile(char*file)
{

	
		
	
	//创建一个XML的文档对象。
	TiXmlDocument *myDocument = new TiXmlDocument();
	//创建一个根元素并连接。
	TiXmlElement *RootElement = new TiXmlElement("Taints");
	myDocument->LinkEndChild(RootElement);
	//创建一个Person元素并连接。
	Node * p = head;
	while(p!=NULL)
	{
	TiXmlElement *PersonElement = new TiXmlElement("Taint");
	RootElement->LinkEndChild(PersonElement);
	//设置Person元素的属性。
	//PersonElement->SetAttribute("type", p->Type);
	//创建name元素、age元素并连接。
	TiXmlElement *NameElement = new TiXmlElement("ID");
	TiXmlElement *AgeElement = new TiXmlElement("line");
	TiXmlElement *functionname = new TiXmlElement("function");
	TiXmlElement *file_name = new TiXmlElement("file");
	PersonElement->LinkEndChild(NameElement);
	PersonElement->LinkEndChild(AgeElement);
	PersonElement->LinkEndChild(functionname);
	PersonElement->LinkEndChild(file_name);
	//设置name元素和age元素的内容并连接。
	TiXmlText *NameContent = new TiXmlText(p->TYPE);
	TiXmlText *AgeContent = new TiXmlText(p->line);
	TiXmlText *functname = new TiXmlText(p->Tfunction);
	TiXmlText *file_name_1 = new TiXmlText(p->Tfile);
	NameElement->LinkEndChild(NameContent);
	AgeElement->LinkEndChild(AgeContent);
	functionname->LinkEndChild(functname);
	file_name->LinkEndChild(file_name_1);
	p = p->next;
	}
	//保存到文件


	myDocument->SaveFile(file);
	return 1;
}
/*输出污染表*/
void Ttable::outTtable()
{

}
/*插入节点，对外函数*/
void Ttable::insert(string  line, string function, int TYPE)
{
	
	
	Node *t = makeTnode(line, function, TYPE);
	if (head == NULL)
	{
		
		head = t;
		head->next = NULL;
	}
	else
	{
		
		t->next = head;
		head = t;
	}
	return ;
}
Ttable::Node* Ttable::makeTnode(string&  line, string& function, int TYPE)
{
	
	/*--------------------------------*/
	int num = line.find(':', 2);
	string test_line;
	int num2 = line.find(':', num+1);
	test_line.insert(0, line, num + 1, num2);
//	cout << test_line << endl;
	string test_file;
	test_file.insert(0, line, 0, num);
//	cout << test_file << endl;
	/*以上部分就是将参数line进行解析，得到了代表行号的参数，和文件路径*/
	/*-------------------------------*/

	//测试
	//	Node * t = NULL;
	//	t = exchange(p, line, function);
	//将行号转换为char类型

	Node * t = new Node;
	/*--------------------------------*/
	t->line = new char[50];
	//	sprintf(t->line, "%d", line);
	string str_line = test_line;
	str_line.copy(t->line, str_line.length(), 0);
	*(t->line + str_line.length()) = '\0';
//	cout << t->line << "t->line" << endl;
	/*将第一部分解释的行号进行转换，存入节点数据域中*/
	/*-----------------------------------*/

	//将名字转换为char类型
	//cout << p->getQualifiedNameAsString().data()<<" 23333" << endl;
	//cout <<(char*) p->getQualifiedNameAsString().data() << " 1111" << endl;
	/*
	t->Tname = new char[50];
	t->Tname = NULL;
	*/
	//	string test = (char*)p->getQualifiedNameAsString().data();
	//t->Tname = (char*)test.data();
	//	test.copy(t->Tname, test.length(), 0);
	//	*(t->Tname + test.length()) = '\0';
	//	cout << t->Tname << " t->Tname" << endl;
	//t->Tname = (char*)p->getQualifiedNameAsString().data();
	//cout <<t->Tname <<" 344444" << endl;
	/*为什么不用getDeclName函数获取变量的名字呢,huozhe shi p->getNameAsString*/
	//将函数名字转换为char

	//cout << funtion_name << 111212313 << endl;
	//t->Tfunction = (char*)funtion_name.data();

	/*---------------------------------*/
	t->Tfunction = new char[100];
	string funtion_name = function;
	funtion_name.copy(t->Tfunction, funtion_name.length(), 0);
	*(t->Tfunction + funtion_name.length()) = '\0';
//	cout << t->Tfunction << " Tfunction" << endl;
	/*将输入的字符串参数存入节点*/
	/*------------------------------------*/


	//通过clang得到了该变量所在的文件名。
	//	t->Tfile = (char*)p->getASTContext().getSourceManager().getFilename(p->getSourceRange().getBegin()).data();

	/*----------------------------------------*/
	t->Tfile = new char[500];
	string Tfile_name = test_file;
	Tfile_name.copy(t->Tfile, Tfile_name.length(), 0);
	*(t->Tfile + Tfile_name.length()) = '\0';
//	cout << t->Tfile << " Tfile" << endl;
	/*将文件名存入节点*/
	/*-------------------------------------*/

	t->TYPE = new char[100];
	if (TYPE == 1)
	{
		string Type = "TTYPE_ARRAY";
		Type.copy(t->TYPE, Type.length(), 0);
		*(t->TYPE + Type.length()) = '\0';
	}
	else if (TYPE == 2)
	{
		string Type = "TTYPE_DIV_MOD";
		Type.copy(t->TYPE, Type.length(), 0);
		*(t->TYPE + Type.length()) = '\0';
	}
	else if (TYPE == 3)
	{
		string Type = "TTYPE_LOOP_BOUND";
		Type.copy(t->TYPE, Type.length(), 0);
		*(t->TYPE + Type.length()) = '\0';
	}
	else if (TYPE == 4)
	{
		string Type = "TTYPE_MEM";
		Type.copy(t->TYPE, Type.length(), 0);
		*(t->TYPE + Type.length()) = '\0';
	}
	else if (TYPE == 5)
	{
		string Type = "TTYPE_POINTER_NULL_SET";
		Type.copy(t->TYPE, Type.length(), 0);
		*(t->TYPE + Type.length()) = '\0';
	}
	else if (TYPE == 6)
	{
		string Type = "TTYPE_RECURSIVE_CALL";
		Type.copy(t->TYPE, Type.length(), 0);
		*(t->TYPE + Type.length()) = '\0';
	}

	return t;

}
Ttable::Node* Ttable::exchange(string&  line, string& function, int TYPE)
{
	//将行号转换为char类型
	Node * t = new Node;
	t->line = new char[50];
	sprintf(t->line, "%d", line);
	//将名字转换为char类型
	//	t->Tname = (char*)p->getQualifiedNameAsString().data();
	/*为什么不用getDeclName函数获取变量的名字呢,huozhe shi p->getNameAsString*/
	//将函数名字转换为char
	t->Tfunction = (char*)function.data();
	//通过clang得到了该变量所在的文件名。
	//	t->Tfile = (char*)p->getASTContext().getSourceManager().getFilename(p->getSourceRange().getBegin()).data();
	return t;

}
void Ttable::listout()
{
	
	Node * t = NULL;
	t = head;
	while (t != NULL)
	{
		
		//cout << t->Tname << endl;
		cout << t->line << endl;
		cout << t->Tfunction << endl;
		cout << t->Tfile << endl;
		cout << endl;
		t = t->next;
	}
}
bool Ttable::XMLout(string file_addr)
{

	if (head == NULL)
	{
		cout << "Ttable is null!" << endl;
		
		return 0;
	}
	else
	{
		char * file = (char*)file_addr.data();
		//char *file = "C:/Users/dell/Desktop/111/Warning%d.XML";
		char * f = NULL;
		int i = 1;
		Node*p = head;
		CreateXmlFile(file);
		/*
		while (p != NULL)
		{
			//cout << i << endl;	
			f = new char[50];
			sprintf(f, file, i);
			CreateXmlFile(p, f);
			i++;
			p = p->next;
		}
		*/
		return 1;
	}

}
