%{
#include <stdio.h>
#include <string.h>
#include "TeaplAst.h"
#include <string.h>
#include "y.tab.hpp"
extern "C" {void yyerror(char*);}
extern int line, col;
void set(int a){
  printf("hahaha %d %d\n",yylval.pos->line, a);
}
%}


%x COMMENT1
%x COMMENT2

/* TODO: */
/* your lexer*/
%%
"//"			{col+=2; BEGIN COMMENT1; }
<COMMENT1>[\n\r]	{line++;col=1;BEGIN INITIAL; }
<COMMENT1>.		{col++;}
"/*"			{col+=2;BEGIN COMMENT2;}
<COMMENT2>"*/"		{col+=2;BEGIN INITIAL;}
<COMMENT2>[\n\r]	{line++;col=1;}
<COMMENT2>.		{col++;}

"fn"		{yylval.pos = A_Pos(line,col); col+=2; return FN;}
"let"		{yylval.pos = A_Pos(line,col); col+=3; return LET;}
"ret"		{yylval.pos = A_Pos(line,col); col+=3; return RETURN;}
"if"		{yylval.pos = A_Pos(line,col); col+=2; return IF;}
"while"		{yylval.pos = A_Pos(line,col); col+=5; return WHILE;}
"continue"	{yylval.pos = A_Pos(line,col); col+=8; return CONTINUE;}
"break"		{yylval.pos = A_Pos(line,col); col+=5; return BREAK;}
"int"		{yylval.pos = A_Pos(line,col); col+=3; return INT;}
"struct"	{yylval.pos = A_Pos(line,col); col+=6; return STRUCT;}
"else"		{yylval.pos = A_Pos(line,col); col+=4; return ELSE;}
[0-9]+			{		
			int n = atoi(yytext); 
			yylval.tokenNum = A_TokenNum(A_Pos(line,col), n ); 
			col+= strlen(yytext);	
			return UNUM;}
[_a-zA-Z][_a-zA-Z0-9]*	{
			 char* id = (char*)malloc(50);
			 strcpy(id,yytext);
			 yylval.tokenId  = A_TokenId(A_Pos(line,col), id); 
			 col+= strlen(yytext);	
                         return ID;}

"+"		{yylval.pos = A_Pos(line,col); col+=1; return ADD;}
"-"		{yylval.pos = A_Pos(line,col); col+=1; return SUB;}
"*"		{yylval.pos = A_Pos(line,col); col+=1; return MUL;}
"/"		{yylval.pos = A_Pos(line,col); col+=1; return DIV;}

"&&"		{yylval.pos = A_Pos(line,col); col+=2; return AND;}
"||"		{yylval.pos = A_Pos(line,col); col+=2; return OR;}
"!"		{yylval.pos = A_Pos(line,col); col+=1; return NOT;}
">"		{yylval.pos = A_Pos(line,col); col+=1; return GTR;}
"<"		{yylval.pos = A_Pos(line,col); col+=1; return LES;}
">="		{yylval.pos = A_Pos(line,col); col+=2; return GEQ;}
"<="		{yylval.pos = A_Pos(line,col); col+=2; return LEQ;}
"=="		{yylval.pos = A_Pos(line,col); col+=2; return EQL;}
"!="		{yylval.pos = A_Pos(line,col); col+=2; return NEQ;}

"="		{yylval.pos = A_Pos(line,col); col+=1; return ASSIGN;}
";"		{yylval.pos = A_Pos(line,col); col+=1; return SEMICOLON;}
"->"		{yylval.pos = A_Pos(line,col); col+=2; return RARROW;}
","		{yylval.pos = A_Pos(line,col); col+=1; return COMMA;}
"{"		{yylval.pos = A_Pos(line,col); col+=1; return LBRACE;}
"}"		{yylval.pos = A_Pos(line,col); col+=1; return RBRACE;}
"("		{yylval.pos = A_Pos(line,col); col+=1; return LPAR;}
")"		{yylval.pos = A_Pos(line,col); col+=1; return RPAR;}
"["		yylval.pos = A_Pos(line,col); {col+=1; return LBRACKET;}
"]"		{yylval.pos = A_Pos(line,col); col+=1; return RBRACKET;}
"."		{yylval.pos = A_Pos(line,col); col+=1; return DOT;}
":"		{yylval.pos = A_Pos(line,col); col+=1; return COLON;}
" "		{yylval.pos = A_Pos(line,col); col+=1;}
[\t\b]		{yylval.pos = A_Pos(line,col); col+=4;}
[\n\r]		{yylval.pos = A_Pos(line,col); line++; col=1;}
.		{col++;yyerror("Unknown character!\n");}
%%


