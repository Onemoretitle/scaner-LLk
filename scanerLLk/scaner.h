#pragma once
#ifndef _SCANER
#define _SCANER
#include "dfs.h"

class TScaner {
	private:
		IM t;
		int uk;
	public:
		void GetData(const char*);
		void PutUK(int i);
		int GetUK(void);
		void PrintError(const char*, char*);
		void PrintError(const char* err, char* a, int t);
		int scaner(LEX l);
		TScaner(const char*);
		~TScaner() {}
};
#endif // !_SCANER