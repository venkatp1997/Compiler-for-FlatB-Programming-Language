%{
#include"ast.h"

#include "parser.tab.h"
#include <bits/stdc++.h>
#define YY_DECL extern "C" int yylex()
using namespace std;
int x = 0;
%}

%%
declblock	return declaration_list;
codeblock		return statement_list;
for return FOR;
if 	return IF;
else return ELSE;
goto return GOTO;
while return WHILE;
read	return READ;
print return PRINT;
println return PRINTLN;
int return INT;


\".*\"	{ 
    		yytext[yyleng-1] = '\0';
			yylval.str = strdup(yytext+1);
			return STRING; 
		}

[0-9][0-9]* {
 				yylval.num = atoi(yytext);
				return NUMBER; 
			} 
[_a-zA-Z][_a-zA-Z0-9]*  {
							yylval.str = strdup(yytext);
							return IDENTIFIER; 
						}
"+"		return '+';
"-"		return '-';
"*"		return '*';
"/"		return '/';
"{"		return '{';
"}"		return '}';
"["		return '[';
"]"		return ']';
"="		return '=';
">="	return GTE;
">"		return GT;
"<="	return LTE;
"<"		return LT;
"=="	return DE;
"!=" 	return NE;
":"		return ':';
";"		return ';';
","		return ',';
[\n]	{x++;}

[ \t]		{ /* Do nothing */ }
.		{ 
		  printf("Unexpected token encountered: %s\n", yytext); 
		  return ETOK;
		}
