#include "scaner.h"

IM t; // исходный модуль
int uk; // указатель текущей позиции в ИМ.
int line = 1, pos = 1; //строка, позиция
int tmpLine = 1, tmpPos = 1;

TScaner::TScaner(const char* FileName) 
{
	GetData(FileName);
	PutUK(0);
}

LEX Keyword[MaxKeyw] = { "if", "else", "int", "double", "public","private", "return", "class" };
int IndexKeyword[MaxKeyw] = { TIf, TElse, TInt, TDouble, TPublic, TPrivate, TReturn, TClass };

void TScaner::GetData(const char* FileName)
{
	FILE* in = fopen(FileName, "r");
	int i = 0;
	while (!feof(in))
		fscanf(in, "%c", &t[i++]);
	t[i] = '\0'; // приписываем знак ’\0’ в конец текста
	fclose(in);
}

void TScaner::PutUK(int i)
{
	line = tmpLine;
	pos = tmpPos;
	uk = i;
} //восстановить указатель

int TScaner::GetUK(void)
{
	tmpLine = line;
	tmpPos = pos;
	return uk;
} // запомнить указатель

void TScaner::PrintError(const char* err, char* a) // выдать сообщение об ошибке
{
	if (a[0] == '\0')
		printf("Ошибка: %s %s\n Позиция: %d, Строка: %d", err, a, pos, line);
	else
		printf("Ошибка : %s. Неверный символ %s\n Позиция: %d, Строка: %d", err, a, pos, line);
	exit(0);
}

void TScaner::PrintError(const char* err, char* a, int t) {
	char expected[40];
	switch (t)
	{
	case TInt:
		strcpy_s(expected, "Ожидалось определение типа");
		break;
	case TDouble:
		strcpy_s(expected, "Ожидалось определение типа");
		break;
	case TReturn:
		strcpy_s(expected, "Ожидался оператор return");
		break;
	case TIdent:
		strcpy_s(expected, "Ожидался идентификатор");
		break;
	case TConstInt:
		strcpy_s(expected, "Ожидалась константа");
		break;
	case TConstStr:
		strcpy_s(expected, "Ожидалась константа");
		break;
	case TComma:
		strcpy_s(expected, "Ожидался символ ,");
		break;
	case TSemicolon:
		strcpy_s(expected, "Ожидался символ ;");
		break;
	case TOBracket:
		strcpy_s(expected, "Ожидался символ (");
		break;
	case TCBracket:
		strcpy_s(expected, "Ожидался символ )");
		break;
	case  TOBrace:
		strcpy_s(expected, "Ожидался символ {");
		break;
	case  TCBrace:
		strcpy_s(expected, "Ожидался символ }");
		break;
	case TIf:
		strcpy_s(expected, "Ожидался оператор if");
		break;
	case TElse:
		strcpy_s(expected, "Ожидался оператор else");
		break;
	case TPlus:
		strcpy_s(expected, "Ожидался символ +");
		break;
	case TMinus:
		strcpy_s(expected, "Ожидался символ -");
		break;
	case TMult:
		strcpy_s(expected, "Ожидался символ *");
		break;
	case TDiv:
		strcpy_s(expected, "Ожидался символ /");
		break;
	case TMod:
		strcpy_s(expected, "Ожидался символ %");
		break;
	case TSave:
		strcpy_s(expected, "Ожидался символ =");
		break;
	case TLess:
		strcpy_s(expected, "Ожидался символ <");
		break;
	case TMore:
		strcpy_s(expected, "Ожидался символ >");
		break;
	case TEQLess:
		strcpy_s(expected, "Ожидался символ <=");
		break;
	case TEQMore:
		strcpy_s(expected, "Ожидался символ >=");
		break;
	case TEQ:
		strcpy_s(expected, "Ожидался символ ==");
		break;
	case TNEQ:
		strcpy_s(expected, "Ожидался символ !=");
		break;
	case TEnd:
		strcpy_s(expected, "Ожидался конец текста");
		break;
	default:
		break;
	}
	printf("Ошибка(строка %d, позиция %d) : %s %s %s\n", line, pos, err, a, expected);
	printf("\n\n");
	system("pause");
	exit(0);

}

int TScaner::scaner(LEX l)
{
	int typ; // тип лексемы
	int i; // текущая длина лексемы
	for (i = 0; i < MaxLex; i++)
	{
		l[i] = 0; //очистили поле лексемы
	}
	
	i = 0; // лексема заполняется с позиции i
	
	start: while ((t[uk] == ' ') || (t[uk] == '\n') || (t[uk] == '\t') || (t[uk] == -51))
	{
		if (t[uk] == '\n') 
		{
			line++;
			pos = 1;
		}
		uk++;
	}
	// пропуск незначащих элементов
	
	if (t[uk] == '/' && t[uk + 1] == '/') 
	{
		uk += 2;
		while (t[uk] != '\n' && t[uk] != '\0')
		{
			uk++;
			pos++;
		}
		goto start;
	}

	if (t[uk] == '\0') 
	{ 
		l[0] = '\0'; 
		return TEnd; 
	}
	else
	{
		if ((t[uk] <= '9') && (t[uk] >= '0'))
		{
			STEP
			while ((t[uk] <= '9') && (t[uk] >= '0')){
				STEP
			}
		return TConstInt;
	}
	else if ((t[uk] == '\'')) {
			STEP
				if (t[uk] == '\'') {
					STEP
					PrintError("Пустая константа", l);
					return TErr;
				}
				else if (t[uk] == '\\') 
				{
					STEP
					if (t[uk] == '\'') {
						STEP
						PrintError("Неверный символ",l);
					}
					else {
						goto poop;
					}
				}
				else {
					poop: STEP
					if (t[uk] == '\'')
					{
						STEP
						return TConstStr;
					}
					else 
					{
						STEP
						PrintError("Символьная константа содержит более одного символа\n или требуется закрывающая кавычка",l);
					}
				}	
		}
	else if ((t[uk] >= 'a') && (t[uk] <= 'z') || (t[uk] >= 'A') && (t[uk] <= 'Z'))
	{
		STEP
			while ((t[uk] <= '9') && (t[uk] >= '0') || (t[uk] >= 'a') && (t[uk] <= 'z') || (t[uk] >= 'A') && (t[uk] <= 'Z')) {
				STEP
			}
		for (int j = 0; j < MaxKeyw; j++) {
			if (strcmp(l, Keyword[j]) == 0)
			{
				return IndexKeyword[j];
			}
		}
		return TIdent;
	}
	else if (t[uk] == '+') 
	{
		STEP
		return TPlus;
	}
	else if (t[uk] == '-') 
	{
		STEP
		return TMinus;
	}
	else if (t[uk] == '*')
	{
		STEP
		return TMult;
	}
	else if (t[uk] == '/')
	{
		STEP
		return TDiv;
	}
	else if (t[uk] == '%')
	{
		STEP
		return TMod;
	}
	else if (t[uk] == ',')
	{
		STEP
		return TComma;
	}
	else if (t[uk] == ';')
	{
		STEP
		return TSemicolon;
	}
	else if (t[uk] == '{')
	{
		STEP
		return TOBrace;
	}
	else if (t[uk] == '}')
	{
		STEP
		return TCBrace;
	}
	else if (t[uk] == '(')
	{
		STEP
		return TOBracket;
	}
	else if (t[uk] == ')')
	{
		STEP
		return TCBracket;
	}
	else if (t[uk] == '<')
	{
		STEP
		if (t[uk] == '=') 
		{
			STEP
			return TEQLess;
		}
		else if (t[uk] == '<') 
		{
			STEP
			return TShiftB;
		}
		else return TLess;
	}
	else if (t[uk] == '>') 
	{
		STEP
		if (t[uk] == '=') 
		{
			STEP
			return TEQMore;
		}
		else if (t[uk] == '>')
		{
			STEP
				return TShiftF;
		}
		else return TMore;
	}
	else if(t[uk] == '=')
	{
		STEP
		if (t[uk] == '=')
		{
			STEP
			return TEQ;
		}
		else return TSave;
	}
	else if (t[uk] == '!') 
	{
		STEP
		if (t[uk] == '=')
		{
			STEP
			return TNEQ;
		}
		else
		{
			PrintError("Неверный символ", l);
			return TErr;
		}
	}
	else {
		PrintError("Неизвестный символ", l); // ошибка
		//return TErr;
		exit(0);
	}

	}
} // конец Scaner


