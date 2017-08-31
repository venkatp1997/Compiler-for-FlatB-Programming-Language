%{
  #include <stdio.h>
  #include <stdlib.h>
  int yylex(void);
  int yyparse(void);
  extern FILE* yyin;
  extern int x;
  void yyerror(const char *s);

%}
%start program
%token declaration_list
%token statement_list
%token NUMBER
%token IDENTIFIER
%token ETOK
%token INT
%token FOR
%token IF
%token ELSE
%token GOTO
%token WHILE
%token GT
%token GTE
%token LT
%token LTE
%token DE
%token NE
%token STRING
%token PRINT
%token PRINTLN
%token READ
%left LT LTE GT GTE 
%left DE NE
%left '+' '-'
%left '*' '/'
%%

program :	declaration_list decl_block statement_list code_block

decl_block :	'{' decl_statements '}'
						|	'{' '}'

decl_statements	:	decl_statement
								| decl_statements decl_statement

decl_statement	: INT DIDS ';'
								| ';'

DIDS	:	IDENTIFIER '[' NUMBER ']'
			| IDENTIFIER
			| DIDS ',' IDENTIFIER
			| DIDS ',' IDENTIFIER '[' NUMBER ']'

code_block :	'{' code_statements '}'
					 |	'{' '}'

code_statements	:	code_statement
								|	code_statements code_statement

code_statement	: forloop
								| whileloop
								| ifc
								|	out ';'
								|	equals ';'
								|	scan ';'
								| gotoc ';'
								| IDENTIFIER ':'
								|	';'
forloop : FOR IDS '=' NUMBER ',' NUMBER code_block
				| FOR IDS '=' NUMBER ',' NUMBER ',' NUMBER code_block
whileloop : WHILE boolexpr code_block
ifc : IF boolexpr code_block ELSE code_block
		| IF boolexpr code_block
gotoc	:	GOTO IDENTIFIER IF boolexpr
			| GOTO IDENTIFIER
equals : IDS '=' expr
scan : READ IDS
out	:	PRINT statement
			| PRINTLN statement
statement	: prints
					| statement ',' prints
prints	:	STRING
				| expr
expr	: term
			| expr '+' expr 
			|	expr '*' expr 
			|	expr '-' expr 
			|	expr '/' expr 

boolexpr	: expr GT expr
					|	expr GTE expr
					|	expr LT expr
					|	expr LTE expr
					|	expr DE expr
					|	expr NE expr
term	:	IDS
			| NUMBER
IDS	:	IDENTIFIER 
		| IDENTIFIER '[' expr ']'

%%

void yyerror (char const *s)
{
       fprintf (stderr, "%s\n", s);
       printf("THE ERROR IS AT: %d\n", x);
}

int main(int argc, char *argv[])
{
	if (argc == 1 ) {
		fprintf(stderr, "Correct usage: bcc filename\n");
		exit(1);
	}

	if (argc > 2) {
		fprintf(stderr, "Passing more arguments than necessary.\n");
		fprintf(stderr, "Correct usage: bcc filename\n");
	}

 yyin = fopen(argv[1], "r");

	yyparse();
}
