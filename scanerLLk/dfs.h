#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define MaxText 10000 //максимальная длина текста ИМ
#define MaxLex 20 //максимальная длина лексемы
#define MaxKeyw 8

#define TIf 1
#define TElse 2
#define TInt 3
#define TDouble 4
#define TPublic 5
#define TPrivate 6
#define TReturn 7

#define TIdent 10
#define TClass 11

#define TConstInt 20
#define TConstStr 21

#define TComma 30
#define TSemicolon 31
#define TOBracket 32
#define TCBracket 33
#define TOBrace 34
#define TCBrace 35
	
#define TPlus 40
#define TMinus 41	
#define TMult 42
#define TDiv 43
#define TMod 44
#define TShiftF 45
#define TShiftB 46
#define TSave 47
#define TLess 48
#define TMore 49
#define TEQLess 50
#define TEQMore 51
#define TEQ 52
#define TNEQ 53

#define TErr 60
#define TEnd 61

#define ntS 100
#define ntD 101
#define ntT 102
#define ntAR 103
#define ntAR1 104
#define ntFS 105
#define ntFS1 106
#define ntSS 107
#define ntSS1 108
#define ntTS 109
#define ntTS1 110
#define ntFOS 111
#define ntFOS1 112
#define ntFFS 113
#define ntL 114
#define ntL1 115
#define ntLO 116
#define ntLD 117

#define dlStartDec 118
#define dlEndDec 119
#define dlSetClass 120
#define dlSetFunc 121
#define dlSetVar 122
#define dlSetNewlvl 123
#define dlRetFromlvl 124
#define dlFind 125
#define dlPush 126
#define dlUpdateVar 127
#define dlRetFunc 128
#define dlGetConst 129
#define dlPlus 130
#define dlMinus 131
#define dlMult 132
#define dlDiv 133
#define dlMod 134
#define dlShiftF 135
#define dlShiftB 136


#define STEP l[i++] = t[uk++]; pos++;

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

typedef char IM[MaxText]; // текст ИМ
typedef char LEX[MaxLex]; // лексема.
//#define _CRT_SECURE_NO_WARNINGS
