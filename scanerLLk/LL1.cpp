//************************************************************
// LL1.cpp -- LL(1)-анализатор до первой ошибки
//************************************************************
#include "LL1.h"

// магазин LL(1)-анализатора и указатель магазина
void LL1::epsilon(){}

int LL1::LL(void) 
{
	//инициализация семантического дерева
	Tree* Root;
	Node* b = new Node();
	memcpy(&b->id, &"EMPTY", strlen("EMPTY") + 1); b->data.dataType = TYPE_EMPTYDATA; b->NodeType = TYPE_EMPTYNODE;
	Root = new Tree(NULL, NULL, NULL, b);
	Root->SetCur(Root);

	int t, fl = 1, i, uk1, ttt;
	LEX l, lll;
	char sss[30];
	
	m[z] = TEnd;
	m[++z] = ntS; // все нетерминалы указаны в defs.hpp
	t = sc->scaner(l);
	while (fl)
	{
		if (!(100 <= m[z] && m[z] <= 117) && !(118 <= m[z] && m[z] <= 136)) //в верхушке магазина терминал
		{				
			if (m[z] == t)
			{
				// верхушка совпадает с отсканированным терминалом
				if (t == TEnd)
				{
					fl = 0; // конец работы
				}
				else
				{
					t = sc->scaner(l); // сканируем новый символ и

					if (t == TIdent)
						strcpy_s(global_id, l);
					if (t == TConstInt || t == TConstStr)
						global_const_int = atoi(l);

					m[z] = 0;
					z--; // стираем верхушку магазина
				}
			}
			else
			{ // обнаружена ошибка
				sc->PrintError("Неверный символ ", l, m[z]);
				return -1; // ожидался символ типа m[z], a не l
			}
		}
		else if (100 <= m[z] && m[z] <= 117)
		{ // в верхущке магазина нетерминал
				switch (m[z]) // по нетерминалу в верхушке магазина
				{ // выполним действия в соответствии таблицей
				case ntS:
				{
					// S → public <D> | private <D> | ɛ
					if (t != TEnd)
					{
						if (t == TPublic)
						{
							m[z++] = ntD;
							m[z++] = TPublic;
						}
						else if (t == TPrivate)
						{
							m[z++] = ntD;
							m[z++] = TPrivate;
						}
					}
					else
					{
						epsilon();
					}
					break;
				}
				case ntD:
				{
					// D → class TIdent { <S> } | int TIdent <T> | double TIdent <T> | TIdent <T>
					switch (t)
					{
					case TIdent:
					{
						// D -> TIdent <T>
						m[z++] = ntT;
						m[z++] = TIdent;
						break;
					}
					case TClass:
					{
						// D -> class TIdent(){ <S> }
						m[z++] = TCBrace;
						m[z++] = ntS;
						m[z++] = TOBrace;
						m[z++] = dlSetClass;
						m[z++] = TIdent;
						m[z++] = dlStartDec;
						m[z++] = TClass;
						break;
					}
					case TDouble:
					{
						m[z++] = ntT;
						m[z++] = TIdent;
						m[z++] = dlStartDec;
						m[z++] = TDouble;
						break;
					}
					case TInt:
					{
						m[z++] = ntT;
						m[z++] = TIdent;
						m[z++] = dlStartDec;
						m[z++] = TInt;
						break;
					}
					}
					break;
				}
				case ntT:
				{
					// T → () { <LD> } <S> | =  <AR> ; <S> | ; <S> | , <D>
					switch (t)
					{
					case TOBracket:
					{
						m[z++] = ntS;
						m[z++] = dlEndDec;
						m[z++] = dlRetFromlvl;
						m[z++] = TCBrace;
						m[z++] = ntLD;
						m[z++] = TOBrace;
						m[z++] = dlSetNewlvl;
						m[z++] = TCBracket;
						m[z++] = TOBracket;
						m[z++] = dlSetFunc;
						break;
					}
					case TSave:
					{
						m[z++] = ntS;
						m[z++] = dlEndDec;
						m[z++] = TSemicolon;
						m[z++] = ntAR;
						m[z++] = TSave;
						m[z++] = dlSetVar;
						break;
					}
					case TComma:
					{
						m[z++] = ntD;
						m[z++] = TComma;
						m[z++] = dlSetVar;
						break;
					}
					case TSemicolon:
					{
						m[z++] = ntS;
						m[z++] = dlEndDec;
						m[z++] = TSemicolon;
						m[z++] = dlSetVar;
						break;
					}
					}
					break;
				}
				case ntAR:
				{
					// AR → <FS> <AR’> 
					m[z++] = ntAR1;
					m[z++] = ntFS;
					break;
				}
				case ntAR1:
				{
					// AR’ → + <FS><AR’> | – <FS><AR’> | ɛ
					switch (t)
					{
					case TPlus:
					{
						m[z++] = ntAR1;
						m[z++] = dlPlus;
						m[z++] = ntFS;
						m[z++] = TPlus;
						break;
					}
					case TMinus:
					{
						m[z++] = ntAR1;
						m[z++] = dlMinus;
						m[z++] = ntFS;
						m[z++] = TMinus;
						break;
					}
					default:
					{
						epsilon();
					}
					}
					break;
				}
				case ntFS:
				{
					// FS → <SS> <FS’>
					m[z++] = ntFS1;
					m[z++] = ntSS;
					break;
				}
				case ntFS1:
				{
					// FS’ → * <SS><FS’> | / <SS><FS’> | % <SS><FS’> | ɛ
					switch (t)
					{
					case TMult:
					{
						m[z++] = ntFS1;
						m[z++] = dlMult;
						m[z++] = ntSS;
						m[z++] = TMult;
						break;
					}
					case TDiv:
					{
						m[z++] = ntFS1;
						m[z++] = dlDiv;
						m[z++] = ntSS;
						m[z++] = TDiv;
						break;
					}
					case TMod:
					{
						m[z++] = ntFS1;
						m[z++] = dlMod;
						m[z++] = ntSS;
						m[z++] = TMod;
						break;
					}
					default:
					{
						epsilon();
					}
					}
					break;
				}
				case ntSS:
				{
					// SS → <TS><SS’>
					m[z++] = ntSS1;
					m[z++] = ntTS;
					break;
				}
				case ntSS1:
				{
					// SS’ → >> <TS><SS’> | << <TS><SS’> | ɛ
					switch (t)
					{
					case TShiftF:
					{
						m[z++] = ntSS1;
						m[z++] = dlShiftF;
						m[z++] = ntTS;
						m[z++] = TShiftF;
						break;
					}
					case TShiftB:
					{
						m[z++] = ntSS1;
						m[z++] = dlShiftB;
						m[z++] = ntTS;
						m[z++] = TShiftB;
						break;
					}
					default:
					{
						epsilon();
					}
					}
					break;
				}
				case ntTS:
				{
					// TS → <FOS><TS’>
					m[z++] = ntTS1;
					m[z++] = ntFOS;
					break;
				}
				case ntTS1:
				{
					// TS’ → < <FOS><TS’> | > <FOS><TS’> | >= <FOS><TS’> | <= <FOS><TS’>| ɛ 
					switch (t)
					{
					case TLess:
					{
						m[z++] = ntTS1;
						m[z++] = ntFOS;
						m[z++] = TLess;
						break;
					}
					case TMore:
					{
						m[z++] = ntTS1;
						m[z++] = ntFOS;
						m[z++] = TMore;
						break;
					}
					case TEQLess:
					{
						m[z++] = ntTS1;
						m[z++] = ntFOS;
						m[z++] = TEQLess;
						break;
					}
					case TEQMore:
					{
						m[z++] = ntTS1;
						m[z++] = ntFOS;
						m[z++] = TEQMore;
						break;
					}
					default:
					{
						epsilon();
					}
					}
					break;
				}
				case ntFOS:
				{
					// FOS → <FFS>><FOS’>  
					m[z++] = ntFOS1;
					m[z++] = ntFFS;
					break;
				}
				case ntFOS1:
				{
					// FOS’ → == <FFS><FOS’> | != <FFS>><FOS’> | ɛ
					switch (t)
					{
					case TEQ:
					{
						m[z++] = ntFOS1;
						m[z++] = ntFFS;
						m[z++] = TEQ;
						break;
					}
					case TNEQ:
					{
						m[z++] = ntFOS1;
						m[z++] = ntFFS;
						m[z++] = TNEQ;
						break;
					}
					default:
					{
						epsilon();
					}
					}
					break;
				}
				case ntFFS:
				{
					// FFS → Tident | Tident() | const| (<AR>)
					switch (t)
					{
					case TIdent:
					{
						int tmpUk = sc->GetUK();
						int tmp = sc->scaner(l);
						sc->PutUK(tmpUk);
						if (tmp == TOBracket) 
						{
							m[z++] = dlRetFunc;
							m[z++] = dlPush;
							m[z++] = TCBracket;
							m[z++] = TOBracket;
							m[z++] = dlFind;
							m[z++] = TIdent;
						}
						else
						{
							m[z++] = dlPush;
							m[z++] = dlFind;
							m[z++] = TIdent;
						}
						break;
					}
					case TConstInt:
					{
						m[z++] = dlPush;
						m[z++] = dlGetConst;
						m[z++] = TConstInt;
						break;
					}
					case TConstStr:
					{
						m[z++] = dlPush;
						m[z++] = dlGetConst;
						m[z++] = TConstStr;
						break;
					}
					case TOBracket:
					{
						m[z++] = TCBracket;
						m[z++] = ntAR;
						m[z++] = TOBracket;
						break;
					}
					default:
					{
						sc->PrintError("неверный символ", l);
						return -1;
					}
					}
					break;
				}
				case ntL:
				{
					// L → if(<AR>) <LO> <L’>
					m[z++] = ntL1;
					m[z++] = ntLO;
					m[z++] = TCBracket;
					m[z++] = ntAR;
					m[z++] = TOBracket;
					m[z++] = TIf;
					break;
				}
				case ntL1:
				{
					// L’ → ɛ | else <LO>
					if (t != TElse)
					{
						epsilon();
					}
					else
					{
						m[z++] = ntLO;
						m[z++] = TElse;
					}
					break;
				}
				case ntLO:
				{
					// LO → <L> | TIdent = <AR>; | {<LD>}
					switch (t)
					{
					case TIf:
					{
						m[z++] = ntL;
						break;
					}
					case TIdent:
					{
						m[z++] = ntS;
						int tmpUk = sc->GetUK();
						int tmp = sc->scaner(l);
						sc->PutUK(tmpUk);
						if (tmp == TOBracket)
						{
							m[z++] = TSemicolon;
							m[z++] = dlRetFunc;
							m[z++] = dlPush;
							m[z++] = TCBracket;
							m[z++] = TOBracket;
							m[z++] = dlFind;
							m[z++] = TIdent;
						}
						else
						{
							m[z++] = TSemicolon;
							m[z++] = dlUpdateVar;
							m[z++] = ntAR;
							m[z++] = TSave;
							m[z++] = dlFind;
							m[z++] = TIdent;
						}
						break;
					}
					case TOBrace:
					{
						m[z++] = dlRetFromlvl;
						m[z++] = TCBrace;
						m[z++] = ntLD;
						m[z++] = TOBrace;
						m[z++] = dlSetNewlvl;
						break;
					}
					}
					break;
				}
				case ntLD:
				{
					// LD →  ɛ | <LO><LD> | <D><LD>
					switch (t)
					{
					case TPublic:
					{
						m[z++] = ntLD;
						m[z++] = ntS;
						break;
					}
					case TPrivate:
					{
						m[z++] = ntLD;
						m[z++] = ntS;
						break;
					}
					case TIdent:
					{
						m[z++] = ntLD;
						m[z++] = ntLO;
						break;
					}
					case TIf:
					{
						m[z++] = ntLD;
						m[z++] = ntLO;
						break;
					}
					default:
					{
						epsilon();
					}
					}
					break;
				}
				} // конец switch по верхушке магазина
			m[z] = 0;
			z--;
		}
		else if (118 <= m[z] && m[z] <= 136)
		{
		
		DATA_TYPE first, second; Data td;
		switch (m[z])
		{
		case dlStartDec:
			Tree::flagDecl = true;
			break;
		case dlEndDec:
			Tree::flagDecl = false;
			break;
		case dlSetClass:
			Root->SemSetVar(global_id, TYPE_CLASS, TYPE_EMPTYDATA);
			break;
		case dlSetFunc:
			Root->SemSetVar(global_id, TYPE_FUNCT, global_type);
			break;
		case dlSetVar:
			Root->SemSetVar(global_id, TYPE_VAR, global_type);
			break;
		case dlSetNewlvl:
			if (!flag_for) {
				stack_retOper.push(Root->SemSetOper(TYPE_OPERATOR));
			}
			flag_for = false;
			break;
		case dlRetFromlvl:
			if (m[z] != m[z - 1]) 
			{
				Root->SetCur(stack_retOper.top());
				stack_retOper.pop();
			}
			break;
		case dlFind:
			global_type = (Root->CheckID(Root->Cur, global_id)).dataType;
			break;
		case dlPush:
			mt[zt++] = global_type;
			break;
		case dlUpdateVar:
			td.dataType = mt[zt - 1];
			if (type_for_Q == TYPE_INTEGER && mt[zt - 1] == TYPE_DOUBLE)
				PrintWarning("Преобразование double к int. Возможна потеря данных");
			break;
		case dlRetFunc:
			Root->SetCur(retFunc);
			break;
		case dlGetConst:
			global_type = TYPE_INTEGER;
			break;
		case dlPlus:
			first = mt[zt - 2];
			second = mt[zt - 1];
			mt[zt - 2] = TYPE_EMPTYDATA; mt[zt - 1] = TYPE_EMPTYDATA;
			zt -= 2;
			if (first == TYPE_INTEGER) {
				if (second == TYPE_INTEGER) {
					mt[zt] = TYPE_INTEGER;
				}
				else if (second == TYPE_DOUBLE) {
					mt[zt] = TYPE_DOUBLE;
				}
			}
			else if (first == TYPE_DOUBLE) {
				if (second == TYPE_INTEGER) {
					mt[zt] = TYPE_DOUBLE;
				}
				else if (second == TYPE_DOUBLE) {
					mt[zt] = TYPE_DOUBLE;
				}
			}
			break;
		case dlMinus:
			first = mt[zt - 2];
			second = mt[zt - 1];
			mt[zt - 2] = TYPE_EMPTYDATA; mt[zt - 1] = TYPE_EMPTYDATA;
			zt -= 2;
			if (first == TYPE_INTEGER) {
				if (second == TYPE_INTEGER) {
					mt[zt] = TYPE_INTEGER;
				}
				else if (second == TYPE_DOUBLE) {
					mt[zt] = TYPE_DOUBLE;
				}
			}
			else if (first == TYPE_DOUBLE) {
				if (second == TYPE_INTEGER) {
					mt[zt] = TYPE_DOUBLE;
				}
				else if (second == TYPE_DOUBLE) {
					mt[zt] = TYPE_DOUBLE;
				}
			}
			break;
		case dlMult:
			first = mt[zt - 2];
			second = mt[zt - 1];
			mt[zt - 2] = TYPE_EMPTYDATA; mt[zt - 1] = TYPE_EMPTYDATA;
			zt -= 2;
			if (first == TYPE_INTEGER) {
				if (second == TYPE_INTEGER) {
					mt[zt] = TYPE_INTEGER;
				}
				else if (second == TYPE_DOUBLE) {
					mt[zt] = TYPE_DOUBLE;
				}
			}
			else if (first == TYPE_DOUBLE) {
				if (second == TYPE_INTEGER) {
					mt[zt] = TYPE_DOUBLE;
				}
				else if (second == TYPE_DOUBLE) {
					mt[zt] = TYPE_DOUBLE;
				}
			}
			break;
		case dlDiv:
			first = mt[zt - 2];
			second = mt[zt - 1];
			mt[zt - 2] = TYPE_EMPTYDATA; mt[zt - 1] = TYPE_EMPTYDATA;
			zt -= 2;
			if (first == TYPE_INTEGER) {
				if (second == TYPE_INTEGER) {
					mt[zt] = TYPE_INTEGER;
				}
				else if (second == TYPE_DOUBLE) {
					mt[zt] = TYPE_DOUBLE;
				}
			}
			else if (first == TYPE_DOUBLE) {
				if (second == TYPE_INTEGER) {
					mt[zt] = TYPE_DOUBLE;
				}
				else if (second == TYPE_DOUBLE) {
					mt[zt] = TYPE_DOUBLE;
				}
			}
			break;
		case dlMod:
			first = mt[zt - 2];
			second = mt[zt - 1];
			mt[zt - 2] = TYPE_EMPTYDATA; mt[zt - 1] = TYPE_EMPTYDATA;
			zt -= 2;
			if (first == TYPE_DOUBLE || second == TYPE_DOUBLE)
				PrintError("Операнды в операции % должны быть целочисленными", l);
			mt[zt] = TYPE_INTEGER;
			break;
		case dlShiftF:
			first = mt[zt - 2];
			second = mt[zt - 1];
			mt[zt - 2] = TYPE_EMPTYDATA; mt[zt - 1] = TYPE_EMPTYDATA;
			zt -= 2;
			if (first == TYPE_DOUBLE || second == TYPE_DOUBLE)
				PrintError("Операнды в операции сдвигов должны быть целочисленными", l);
			mt[zt] = TYPE_INTEGER;
			break;
		case dlShiftB:
			first = mt[zt - 2];
			second = mt[zt - 1];
			mt[zt - 2] = TYPE_EMPTYDATA; mt[zt - 1] = TYPE_EMPTYDATA;
			zt -= 2;
			if (first == TYPE_DOUBLE || second == TYPE_DOUBLE)
				PrintError("Операнды в операции сдвигов должны быть целочисленными", l);
			mt[zt] = TYPE_INTEGER;
			break;
		}

		m[z] = 0;
		z--;

		}
	} // конец цикла обработки магазина
	
	Root->Print();

	return 1; // нормальный выход - синтаксический анализ закончен
}