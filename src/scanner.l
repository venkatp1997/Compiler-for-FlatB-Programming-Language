%{
#include "y.tab.h"
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
\".*\" return STRING;
[0-9][0-9]*	{  
			printf("Token type: Number, Lexeme/Token Value: %s\n", yytext);  
			return NUMBER;
                }
[a-zA-Z][a-zA-Z0-9]* {  
			printf("Token type: Identifier, Lexeme/Token Value: %s\n", yytext);  
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
