
//************************************************************
// scanerLLk.cpp -- Главная программа транслятора, в которой
// работает LL(1)-анализатор до первой ошибки
//************************************************************
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "dfs.h"
#include "scaner.h"
#include "LL1.h"

using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_CTYPE, "rus");
	int type;
	LEX l;
	TScaner* sc;
	if (argc <= 1)
	{
		sc = new TScaner("input.txt");
	}
	else
	{
		sc = new TScaner(argv[1]);
	}
	sc->PutUK(0); // поставили указатель на начало исходного модуля
	LL1* scl = new LL1(sc);
	scl -> LL();
	return 0;
}
