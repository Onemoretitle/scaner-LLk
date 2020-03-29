#include "semant.h"
#include <string.h>
#include "dfs.h"
#include "semant.h"

int cnt_empty = 1;

void PrintError(const char* err, char* a)
{
	if (a[0] == '\0')
	{
		printf("Ошибка: %s %s\n", err, a);
	}
	else
	{
		printf("Ошибка : %s. Неверный символ %s\n", err, a);
	}
	exit(0);
}

void PrintWarning(const char* war)
{
		printf("Внимание: %s\n", war);
}

Tree* Tree::Cur = (Tree*)NULL;
bool Tree::flagInterpret = true;
bool Tree::haveInitend = false;
bool Tree::flagDecl = false;

				// ФУНКЦИИ ОБРАБОТКИ БИНАРНОГО ДЕРЕВА

// конструктор создает узел с заданными связями и данными
Tree::Tree(Tree* l, Tree* r, Tree* u, Node* d)
{
	n = new Node();
	Up = u; Left = l; Right = r;	// установили ссылки
	memcpy(n, d, sizeof(Node));		// установили данные
}
// конструктор создает новый узел с пустыми связями и данными
Tree::Tree()	
{
	n = new Node();
	Up = NULL; Left = NULL; Right = NULL;
	memcpy(n, &("------"), sizeof(Node));
}

Tree::~Tree()
{
	//delete n->data.dataValue.valArray;
	delete n;
	if (Left != NULL)
		delete Left;
	if (Right != NULL)
		delete Right;
}

void Tree::setFlagInter(bool flag)
{
	flagInterpret = flag;
}

bool Tree::getFlagInter()
{
	return flagInterpret;
}

void Tree::setFlagEnd(bool flag)
{
	haveInitend = flag;
}

bool Tree::getFlagEnd()
{
	return haveInitend;
}
// создать левого потомка от текущей вершины this
void Tree::SetLeft(Node* Data)	
{
	Tree* a = new Tree(NULL, NULL, this, Data);		// новая вершина
	Left = a;										// связали this с новой вершиной
}
// создать правого потомка от текущей вершины this
void Tree::SetRight(Node* Data)
{
	Tree* a = new Tree(NULL, NULL, this, Data);		// новая вершина
	Right = a;										// связали this с новой вершиной
}
// поиск данных в дереве, начиная от текущей вершины this
// до его корня вверх по связям
Tree* Tree::FindUp(LEX id)	
{
	return FindUp(this, id);
}
// поиск данных в дереве от заданной вершины From
// до его корня вверх по связям
Tree* Tree::FindUp(Tree* From, LEX id)	
{
	if (!flagInterpret)
		return NULL;

	Tree* i = From; // текущая вершина поиска
	while ((i != NULL) &&
		(memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Up; // поднимаемся наверх по связям
	return i;
}

Tree* Tree::FindRightLeft(Tree* From, LEX id)
// Поиск прямых потомков From
{
	Tree* i = From->Right;
	while ((i != NULL) &&
		(memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Left;
	return i;
}
// Поиск элемента id вверх по дереву от текущей вершины From.
// Поиск осуществляется на одном уровне вложенности по левым связям
Tree* Tree::FindUpOneLevel(Tree* From, LEX id) {
	Tree* i = From;		// текущая вершина поиска
	while ((i != NULL))
	{
		if (i->Up == NULL)
			break;
		if (i->Up->Right == i)
			break;

		if (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) == 0)
			return i;	// нaшли совпадающий идентификатор
		i = i->Up;		// поднимаемся наверх по связям
	}
	return NULL;
}

void Tree::remRightSubtree()
{
	if (Right != NULL)
		delete Right;
	Right = NULL;
}
// отладочная программа печати дерева
void Tree::Print(void)
{
	printf("Вершина с данными %s ----->", n->id);
	if (Left != NULL) printf(" слева данные %s", Left->n->id);
	if (Right != NULL) printf(" справа данные %s", Right->n->id);
	printf("\n");
	if (Left != NULL) Left->Print();
	if (Right != NULL) Right->Print();
}

				// СЕМАНТИЧЕСКИЕ ПОДПРОГРАММЫ
// установить текущий узел дерева
void Tree::SetCur(Tree* a)
{
	if (a != NULL)
	{
		Cur = a;
	}
}
// получить значение текущего узла дерева
Tree* Tree::GetCur(void)
{
	return Cur;
}
// 1 занесение идентификатора функции, класса или составного оператора "a" в таблицу с типом nt
// создание новго уровня(потомка) с типом EMPTYNODE
Tree* Tree::SemSetFunct(LEX id, NODE_TYPE nt, DATA_TYPE dt)
{
	if (!flagInterpret)
		return NULL;

	if (DupControl(Cur, id))
		PrintError("Повторное описание идентификатора ", id);
	Tree* v; 
	Node b;
	memcpy(b.id, id, strlen(id) + 1);
	b.NodeType = nt;
	b.data.dataType = dt;
	b.pos = Cur;
	b.text = NULL;
	b.InitVar = false;
	Cur->SetLeft(&b); // сделали вершину - функцию
	Cur = Cur->Left;
	v = Cur; // это точка возврата после выхода из функции
	
	char str[15] = "EMPTY";
	char cnt[5];
	sprintf(cnt, "%d", cnt_empty++);
	strcat(str, cnt);

	memcpy(&b.id, &"", strlen(str) + 1);
	b.NodeType = TYPE_EMPTYNODE; 
	b.data.dataType = TYPE_EMPTYDATA;
	Cur->SetRight(&b); // сделали пустую вершину
	Cur = Cur->Right;
	return v;
}

Tree* Tree::SemSetOper(NODE_TYPE nt)
{
	if (!flagInterpret)
		return NULL;

	Tree* v; 
	Node b;

	char str[15] = "EMPTY";
	char cnt[5];
	sprintf(cnt, "%d", cnt_empty++);
	strcat(str, cnt);

	memcpy(&b.id, &str, strlen(str) + 1); 
	b.data.dataType = TYPE_EMPTYDATA; 
	b.NodeType = nt;
	Cur->SetLeft(&b);
	Cur = Cur->Left;
	v = Cur; // точка возврата

	strcpy(str, "EMPTY");
	sprintf(cnt, "%d", cnt_empty++);
	strcat(str, cnt);
	memcpy(&b.id, &str, strlen(str) + 1);

	Cur->SetRight(&b);
	Cur = Cur->Right;
	return v;
}
// 2 установить тип dt для переменной по адресу Addr
void Tree::SemSetType(Tree* Addr, DATA_TYPE dt)
{
	Addr->n->data.dataType = dt;
}

void Tree::SemSetType(Tree* Addr, Data dt)
{
	Addr->n->data = dt;
}

void Tree::SemSetUk(Tree* Addr, int uk)
{
	Addr->n->text = uk;
}

// 3 Проверка на существование переменной ранее на этом уровне. 
//Если есть, то ошибка. Если нет, добавление в таблицу.
Tree* Tree::SemSetVar(LEX id, NODE_TYPE nt, DATA_TYPE dt)
{
	if (DupControl(Cur, id))
		PrintError("Повторное описание идентификатора ", id);
	Node b;
	memcpy(b.id, id, strlen(id) + 1);
	b.NodeType = nt;
	b.data.dataType = dt;
	b.pos = Cur;
	b.text = NULL;
	b.InitVar = false;
	Cur->SetLeft(&b); // сделали вершину - переменную
	Cur = Cur->Left;
	return Cur;
}

void Tree::SetInit(Tree* Vert) 
{
	Vert->n->InitVar = true;
}
// 3.1 Проверка на существование переменной ранее. 
Tree* Tree::SemGetVar(LEX id)
{
	if (!flagInterpret)
		return NULL;

	Tree* v = FindUp(Cur, id);
	if (v == NULL) {
		PrintError("Отсутствует описание идентификатора ", id);
		return nullptr;
	}
	if (v->n->NodeType == NODE_TYPE::TYPE_FUNCT || v->n->NodeType == NODE_TYPE::TYPE_INITFUNCT || v->n->NodeType == NODE_TYPE::TYPE_VAR)
		return v;
	else {
		PrintError("Идентификатор описан не как переменная или функция ", id);
		return nullptr;
	}
}
// 5 проверка на приводимость типов
void Tree::SemGetType(DATA_TYPE  firstRt, DATA_TYPE  secondRt)
{
	
	if (firstRt != secondRt)
	{
		PrintWarning("Несоответвсие типов");
	}
	
}
//вычисляет результат операций над операндами t и g в соответствии с таблицей приведений.
Data Tree::SemGetType(Data  firstRt, Data  secondRt, int oper, LEX id)
{
	if (!flagInterpret)
		return Data();

	Data res = firstRt;

	switch (firstRt.dataType) 
	{
	case DATA_TYPE::TYPE_DOUBLE:
		switch (secondRt.dataType) 
		{
		case DATA_TYPE::TYPE_DOUBLE:
			switch (oper) 
			{
			case TShiftF:
				PrintError("Выражение должно относиться к целочисленному типу или типу перечисления без области видимости", id);
				//res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble >> secondRt.dataValue.DataAsDouble;
				break;
			case TShiftB:
				PrintError("Выражение должно относиться к целочисленному типу или типу перечисления без области видимости", id);
				//res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble << secondRt.dataValue.DataAsDouble;
				break;
			case TPlus:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble + secondRt.dataValue.DataAsDouble;
				break;
			case TMinus:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble - secondRt.dataValue.DataAsDouble;
				break;
			case TMult:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble * secondRt.dataValue.DataAsDouble;
				break;
			case TDiv:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble / secondRt.dataValue.DataAsDouble;
				break;
			case TLess:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble < secondRt.dataValue.DataAsDouble;
				break;
			case TMore:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble > secondRt.dataValue.DataAsDouble;
				break;
			case TEQLess:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble <= secondRt.dataValue.DataAsDouble;
				break;
			case TEQMore:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble >= secondRt.dataValue.DataAsDouble;
				break;
			case TEQ:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble == secondRt.dataValue.DataAsDouble;
				break;
			case TNEQ:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble != secondRt.dataValue.DataAsDouble;
				break;
			}
			break;

		case DATA_TYPE::TYPE_INTEGER:
			switch (oper) 
			{
			case TShiftF:
				PrintError("Выражение должно относиться к целочисленному типу или типу перечисления без области видимости", id);
				//res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble >> secondRt.dataValue.DataAsInteger;
				break;
			case TShiftB:
				PrintError("Выражение должно относиться к целочисленному типу или типу перечисления без области видимости", id);
				//res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble << secondRt.dataValue.DataAsInteger;
				break;
			case TPlus:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble + secondRt.dataValue.DataAsInteger;
				break;
			case TMinus:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble - secondRt.dataValue.DataAsInteger;
				break;
			case TMult:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble * secondRt.dataValue.DataAsInteger;
				break;
			case TDiv:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble / secondRt.dataValue.DataAsInteger;
				break;
			case TLess:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble < secondRt.dataValue.DataAsInteger;
				break;
			case TMore:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble > secondRt.dataValue.DataAsInteger;
				break;
			case TEQLess:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble <= secondRt.dataValue.DataAsInteger;
				break;
			case TEQMore:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble >= secondRt.dataValue.DataAsInteger;
				break;
			case TEQ:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble == secondRt.dataValue.DataAsInteger;
				break;
			case TNEQ:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsDouble != secondRt.dataValue.DataAsInteger;
				break;
			}
			break;
		}
		break;

	case DATA_TYPE::TYPE_INTEGER:
		switch (secondRt.dataType) {
		
		case DATA_TYPE::TYPE_DOUBLE:
			switch (oper) {
			case TShiftF:
				PrintError("Выражение должно относиться к целочисленному типу или типу перечисления без области видимости", id);
				//res.dataValue.DataAsDouble = firstRt.dataValue.DataAsInteger >> secondRt.dataValue.DataAsDouble;
				//res.dataType = DATA_TYPE::TYPE_DOUBLE;
				break;
			case TShiftB:
				PrintError("Выражение должно относиться к целочисленному типу или типу перечисления без области видимости", id);
				//res.dataValue.DataAsDouble = firstRt.dataValue.DataAsInteger << secondRt.dataValue.DataAsDouble;
				//res.dataType = DATA_TYPE::TYPE_DOUBLE;
				break;
			case TPlus:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsInteger + secondRt.dataValue.DataAsDouble;
				res.dataType = DATA_TYPE::TYPE_DOUBLE;
				break;
			case TMinus:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsInteger - secondRt.dataValue.DataAsDouble;
				res.dataType = DATA_TYPE::TYPE_DOUBLE;
				break;
			case TMult:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsInteger * secondRt.dataValue.DataAsDouble;
				res.dataType = DATA_TYPE::TYPE_DOUBLE;
				break;
			case TDiv:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsInteger / secondRt.dataValue.DataAsDouble;
				res.dataType = DATA_TYPE::TYPE_DOUBLE;
				break;
			case TLess:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsInteger < secondRt.dataValue.DataAsDouble;
				res.dataType = DATA_TYPE::TYPE_DOUBLE;
				break;
			case TMore:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsInteger > secondRt.dataValue.DataAsDouble;
				res.dataType = DATA_TYPE::TYPE_DOUBLE;
				break;
			case TEQLess:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsInteger <= secondRt.dataValue.DataAsDouble;
				res.dataType = DATA_TYPE::TYPE_DOUBLE;
				break;
			case TEQMore:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsInteger >= secondRt.dataValue.DataAsDouble;
				res.dataType = DATA_TYPE::TYPE_DOUBLE;
				break;
			case TEQ:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsInteger == secondRt.dataValue.DataAsDouble;
				res.dataType = DATA_TYPE::TYPE_DOUBLE;
				break;
			case TNEQ:
				res.dataValue.DataAsDouble = firstRt.dataValue.DataAsInteger != secondRt.dataValue.DataAsDouble;
				res.dataType = DATA_TYPE::TYPE_DOUBLE;
				break;
			}
			break;

		case DATA_TYPE::TYPE_INTEGER:
			switch (oper) {
			case TShiftF:
				res.dataValue.DataAsInteger = firstRt.dataValue.DataAsInteger >> secondRt.dataValue.DataAsInteger;
				break;
			case TShiftB:
				res.dataValue.DataAsInteger = firstRt.dataValue.DataAsInteger << secondRt.dataValue.DataAsInteger;
				break;
			case TPlus:
				res.dataValue.DataAsInteger = firstRt.dataValue.DataAsInteger + secondRt.dataValue.DataAsInteger;
				break;
			case TMinus:
				res.dataValue.DataAsInteger = firstRt.dataValue.DataAsInteger - secondRt.dataValue.DataAsInteger;
				break;
			case TMult:
				res.dataValue.DataAsInteger = firstRt.dataValue.DataAsInteger * secondRt.dataValue.DataAsInteger;
				break;
			case TDiv:
				res.dataValue.DataAsInteger = firstRt.dataValue.DataAsInteger / secondRt.dataValue.DataAsInteger;
				break;
			case TLess:
				res.dataValue.DataAsInteger = firstRt.dataValue.DataAsInteger < secondRt.dataValue.DataAsInteger;
				break;
			case TMore:
				res.dataValue.DataAsInteger = firstRt.dataValue.DataAsInteger > secondRt.dataValue.DataAsInteger;
				break;
			case TEQLess:
				res.dataValue.DataAsInteger = firstRt.dataValue.DataAsInteger <= secondRt.dataValue.DataAsInteger;
				break;
			case TEQMore:
				res.dataValue.DataAsInteger = firstRt.dataValue.DataAsInteger >= secondRt.dataValue.DataAsInteger;
				break;
			case TEQ:
				res.dataValue.DataAsInteger = firstRt.dataValue.DataAsInteger == secondRt.dataValue.DataAsInteger;
				break;
			case TNEQ:
				res.dataValue.DataAsInteger = firstRt.dataValue.DataAsInteger != secondRt.dataValue.DataAsInteger;
				break;
			}
			break;
		}
		break;
	}
	return res;
}

void Tree::SemAssign(Tree* ident, Data newData, bool itsFunc, LEX id, int t)
{
	if (!flagInterpret)
		return;
	if (ident == NULL) {
		PrintError("Sem2: узел дерева NULL", id);
		return;
	}
	
	ident->setVar(newData);
	printf("\n%s = ", ident->n->id);
	if (ident->n->data.dataType == DATA_TYPE::TYPE_INTEGER)
		printf("%d", ident->n->data.dataValue.DataAsInteger);
	else if (ident->n->data.dataType == DATA_TYPE::TYPE_DOUBLE)
		printf("%lf", ident->n->data.dataValue.DataAsDouble);
	printf("\n");
	//Поставить флаг инициализации
	ident->n->flagInit = true;
}

void Tree::setVar(Data newData)
{
	if (this->n->data.dataType == DATA_TYPE::TYPE_INTEGER) {
		switch (newData.dataType)
		{
		case DATA_TYPE::TYPE_DOUBLE:
			this->n->data.dataValue.DataAsInteger = newData.dataValue.DataAsDouble;
			break;
		case DATA_TYPE::TYPE_INTEGER:
			this->n->data.dataValue.DataAsInteger = newData.dataValue.DataAsInteger;
			break;
		}
	}
	else if (this->n->data.dataType == DATA_TYPE::TYPE_DOUBLE) {
		switch (newData.dataType)
		{
		case DATA_TYPE::TYPE_DOUBLE:
			this->n->data.dataValue.DataAsDouble = newData.dataValue.DataAsDouble;
			break;
		case DATA_TYPE::TYPE_INTEGER:
			this->n->data.dataValue.DataAsDouble = newData.dataValue.DataAsInteger;
			break;
		}
	}
}
// проверка идентификатора а на повторное описание внутри блока.
int Tree::DupControl(Tree* Addr, LEX id)
											
{
	if (FindUpOneLevel(Addr, id) == NULL)
		return 0;
	return 1;
}
// проверка на существование идентификатроа
Data Tree::CheckID(Tree* From, LEX id) {
	
	if (!flagInterpret)
		return Data();

	Tree* v = FindUp(From, id);
	if (v == NULL) {
		PrintError("Необъявленный идентифкатор", id);
		return Data();
	}
	else
		return v->n->data;
}

Tree* Tree::RetNearestFunc(Tree* vert) 
{
	Tree* i = vert; // текущая вершина поиска
	while (i != NULL && i->n->NodeType != TYPE_FUNCT && i->n->NodeType != TYPE_INITFUNCT)
		i = i->Up; // поднимаемся наверх по связям
	return i;
}

Data Tree::buildDataFromConst(int t, LEX l)
{
	Data retData;
	if (t == TConstInt) {		//Целая конст
		retData.dataType = DATA_TYPE::TYPE_INTEGER;
		retData.dataValue.DataAsInteger = atoi(l);		//Целая константа
	}
	else if (t == TConstStr) {	//cbvdjkmyfz конст
		retData.dataType = DATA_TYPE::TYPE_INTEGER;
		char *ct = l;
		retData.dataValue.DataAsInteger = atoi(ct);
	}
	return retData;
}