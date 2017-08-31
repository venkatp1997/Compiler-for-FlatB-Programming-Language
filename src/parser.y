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

program :	declaration_list '{' decl_statements '}' statement_list '{' code_statements '}'



DIDS	:	IDENTIFIER '[' NUMBER ']'
			| IDENTIFIER
			| DIDS ',' IDENTIFIER
			| DIDS ',' IDENTIFIER '[' NUMBER ']'

decl_statements	:	decl_statement
								| decl_statements decl_statement

decl_statement	: INT DIDS ';'
								| ';'
code_statements	:	code_statement
								|	code_statements code_statement

code_statement	: loops
								| ifc
								|	io ';'
								|	equals ';'
								| IDENTIFIER ':'
								|	';'
loops	:	forloop
			|	whileloop
			| gotoc ';'
io		:	out
			| scan
forloop : FOR IDS '=' NUMBER ',' NUMBER '{' code_statements '}'
				| FOR IDS '=' NUMBER ',' NUMBER ',' NUMBER '{' code_statements '}'
whileloop : WHILE boolexpr '{' code_statements '}'
ifc : IF boolexpr '{'	code_statements '}' ELSE '{' code_statements '}'
		| IF boolexpr '{' code_statements '}'
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
       printf("THE ERROR IS AT: %d\n", x + 1);
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
