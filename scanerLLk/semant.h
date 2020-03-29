#pragma once
// ������ Semant.hpp -- ���������� ������������� �����������
#ifndef SEMANT
#define SEMANT
#include "dfs.h"
#include "scaner.h"

#define max(a,b) a<b? b : a

enum  DATA_TYPE {
	TYPE_EMPTYDATA, 
	TYPE_INTEGER, 
	TYPE_DOUBLE, 
	TYPE_PUBLIC, 
	TYPE_PRIVTE
};

enum  NODE_TYPE {
	TYPE_EMPTYNODE,
	TYPE_UNKNOWN, 
	TYPE_VAR, 
	TYPE_FUNCT,
	TYPE_INITFUNCT,
	TYPE_CLASS, 
	TYPE_OPERATOR,
	TYPE_IF, 
	TYPE_ELSE
};

union DataValue
{
	int DataAsInteger;
	double DataAsDouble;
};


struct Data {
	DATA_TYPE dataType;			//��� ��������
	DataValue dataValue;	//��������

	Data() {
		dataType = DATA_TYPE::TYPE_EMPTYDATA;
		dataValue = DataValue();
	}
	Data(DATA_TYPE tp, DataValue val) {
		dataType = tp;
		dataValue = val;
	}

};


void PrintError(const char* err, char* a);	// ������ ��������� �� ������

void PrintWarning(const char* war);			// ������ ��������������

class Tree;

struct Node // ���������� � ������� ������
{
	NODE_TYPE NodeType;
	LEX id;
	Tree* pos;
	int text;
	bool flagInit; 
	bool InitVar;
	Data data;

};

class Tree // ������� ������������� �������
{
public:
	Node* n;					// ���������� �� ������� �������
	Tree* Up, * Left, * Right;	// ��������, ����� � ������ �������
	int step;
	static Tree* Cur;			// ������� ������� ������
	static bool flagInterpret;		// ���� �������������
	static bool haveInitend;
	static bool flagDecl;
	
					   // ������� ��������� ��������� ������
	static void setFlagInter(bool flag);
	
	static bool getFlagInter();

	static void setFlagEnd(bool flag);

	static bool getFlagEnd();
	
	Tree(Tree* l, Tree* r, Tree* u, Node* Data);
	
	Tree();
	
	~Tree();
	
	void SetLeft(Node* Data);
	
	void SetRight(Node* Data);
	
	Tree* FindUp(LEX id);
	
	Tree* FindUp(Tree* From, LEX id);

	Tree* FindRightLeft(Tree* From, LEX id);

	Tree* FindUpOneLevel(Tree* From, LEX id);
	
	void Print(void);
	
						// ������������� ������������
	
	// ���������� ������� ���� ������
	void SetCur(Tree* a);
	
	// �������� �������� �������� ���� ������
	Tree* GetCur(void);			
	
	// ��������� �������������� a � ������� � ����� t
	Tree* SemSetFunct(LEX id, NODE_TYPE nt, DATA_TYPE dt);
	
	Tree* SemSetOper(NODE_TYPE nt);

	// ���������� ��� t ��� ���������� �� ������ Addr
	void SemSetType(Tree* Addr, DATA_TYPE dt);	
	
	void SemSetType(Tree* Addr, Data dt);

	void SemSetUk(Tree* Addr, int uk);

	// ����� � ������� ���������� � ������ a � ������� ������ �� ��������������� ������� ������
	Tree* SemSetVar(LEX id, NODE_TYPE nt, DATA_TYPE dt);
	
	// ����� � ������� ������� � ������ a � ������� ������ �� ��������������� ������� ������
	Tree* SemGetVar(LEX id);
	
	void SetInit(Tree* Vert);

	// �������� �� ������������ �����
	// Tree* SemGetType(LEX a, DATA_TYPE dt);
	void SemGetType(DATA_TYPE firstRt, DATA_TYPE secondRt);
	
	Data SemGetType(Data firstRt, Data secondRt, int oper, LEX id);

	// �������� �������������� � �� ��������� �������� ������ �����
	int DupControl(Tree* Addr, LEX id);
	
	// �������� �� ������������� ��������������
	Data CheckID(Tree* From, LEX id);

	Tree* RetNearestFunc(Tree* vert);

	static Data buildDataFromConst(int t, LEX l);
	void SemAssign(Tree* ident, Data newData, bool itsFunc, LEX id, int t);
	void setVar(Data newData);
	void setItemFunc(Tree* ident, Data newData);
	void remRightSubtree();
};
#endif



