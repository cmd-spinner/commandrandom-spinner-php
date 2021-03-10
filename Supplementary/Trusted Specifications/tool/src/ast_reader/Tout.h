#ifndef TOUT_H
#define TOUT_H

#include<stdio.h>
#include<iostream>
#include<string>
#include"AST.h"
#include"tinyxml.h"
#include"tinystr.h"
using namespace std;
#define TTYPE_ARRAY 1
#define TTYPE_DIV_MOD 2
#define TTYPE_LOOP_BOUND 3
#define TTYPE_MEM 4
#define TTYPE_POINTER_NULL_SET 5
#define TTYPE_RECURSIVE_CALL 6

class XYJ_table
{
public:
	FunctionDecl* fd;
	ASTContext* astc;
	XYJ_table(){}
	XYJ_table(FunctionDecl* f, ASTContext* as)
	{
		fd = f;
		astc = as;
	}
};


class Ttable
{
public:
	struct Node
	{
		//char*Tname;
		char*Tfile;
		char*Tfunction;
		char*line;
		char*TYPE;
		Node * next;
	}node;
	Ttable();
	~Ttable();
	void make_node(const VarDecl *p);
	//	bool insert(const VarDecl *p,  int line,string function);
	//	bool insert(const VarDecl *p,string line,string function,int TYPE);
	void insert(string line, string function, int TYPE);
	void outTtable();
	bool CreateXmlFile(char*file);
	VarDecl * serach(const VarDecl * p);
	void listout();
	bool XMLout(string file_addr);
	Node * head = NULL;
private:



	Node* makeTnode(string& line, string& function, int TYPE);

	Node* exchange(string& line, string& function, int TYPE);
};

#endif
