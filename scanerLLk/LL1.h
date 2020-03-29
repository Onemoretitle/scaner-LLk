#pragma once

#include <stdio.h>
#include <string.h>
#include "dfs.h"
#include "scaner.h"
#include "semant.h"
#include <stack>

class LL1
{
	private:
		TScaner* sc;
		int m[5000], z = 0;
	public:
		LL1(TScaner* s) 
		{
			sc = s;
		}
		~LL1() {};
		void epsilon();
		int LL(void);
		
		// магазин типов и указатель магазина типов
		DATA_TYPE mt[1000]; int zt = 0;

		//глобальные переменные для запомниания.
		LEX global_id;
		DATA_TYPE global_type, type_for_Q;
		Tree* retFunc;
		std::stack<Tree*> stack_retOper;
		std::stack<int> stack_mes;
		int global_const_int;
		bool flag_for;
		int cnt_mesuare = 0;
};

