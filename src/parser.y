%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "ast.h"
  extern "C" int yylex();
  extern "C" int yyparse();
  extern "C" FILE *yyin;
  extern int x;
  void yyerror(const char *s);
  #include "lex.yy.c"

%}
%union
{
	char* str;
	int num;
	Program *p;
	Decl_Statement *ds;
	Code_Statement *cs;
	Code_Statement_Block *csb;
	vector<Code_Statement*> *css;
	GoTo* g;
	Id *id;
	vector<Id*> *ids;
	Print *pr;
	vector<Print*> *ps;
	Expr* e;
	BoolExpr* be;
	Term* t;
}

%type <p> program;
%type <ids> DIDS;
%type <ids> MIDS;
%type <ids> decl_statement;
%type <ds> decl_statements;
%type <csb> code_statements;
%type <cs> code_statement;
%type <cs> loops;
%type <cs> io;
%type <cs> forloop;
%type <cs> whileloop;
%type <cs> ifc;
%type <cs> equals;
%type <cs> scan;
%type <cs> out;
%type <ps> statement;
%type <pr> prints;
%type <e> expr;
%type <be> boolexpr;
%type <t> term;
%type <id> IDS;
%type <str> STRING;
%type <str> IDENTIFIER;
%type <num> NUMBER;
%type <cs> gotoc;

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

program :	declaration_list '{' decl_statements '}' statement_list '{' code_statements '}' {$$ = new Program($3, $7);root = $$;}



DIDS	:	IDS {$$ = new vector<Id*>; $$->push_back($1);}
			| DIDS ',' IDS {$1->push_back($3);$$ = $1;}

decl_statements	:	decl_statement {$$ = new Decl_Statement($1);}
								| decl_statements decl_statement {$1->ids->insert($1->ids->end(), $2->begin(), $2->end());$$ = $1;}

decl_statement	: INT DIDS ';' {$$ = $2;}
								| ';' {$$ = new vector<Id*>;}
code_statements	:	code_statement {$$ = new Code_Statement_Block();$$->add($1);}
							|	code_statement code_statements {$2->statements->insert($2->statements->begin(), $1);$$ = $2;}
								| IDENTIFIER ':' code_statements {visitor->label_table[$1] = *($3->statements);$$ = $3;}

code_statement	: loops {$$ = $1;}
								| ifc {$$ = $1;}
								|	io ';' {$$ = $1;}
								|	equals ';' {$$ = $1;}
							
loops	:	forloop {$$ = $1;}
			|	whileloop {$$ = $1;}
			| gotoc ';' {$$ = $1;}
io		:	out {$$ = $1;}
			| scan {$$ = $1;}
gotoc	:	GOTO IDENTIFIER IF boolexpr {$$ = new GoTo($2, $4);}
			| GOTO IDENTIFIER {$$ = new GoTo($2);}
forloop : FOR IDS '=' NUMBER ',' NUMBER ',' NUMBER '{' code_statements '}' {$$ = new Forloop($2, $4, $6, $10, $8);}
		| FOR IDS '=' NUMBER ',' NUMBER '{' code_statements '}' {$$ = new Forloop($2, $4, $6, $8);}
whileloop : WHILE boolexpr '{' code_statements '}' {$$ = new Whileloop($2, $4);}
ifc : IF boolexpr '{'	code_statements '}' ELSE '{' code_statements '}' {$$ = new IfElse($2, $4, $8);}
		| IF boolexpr '{' code_statements '}' {$$ = new IfElse($2, $4);}
equals : IDS '=' expr {$$ = new Assignment($1, $3);}
scan : READ MIDS {$$ = new Read($2);}
out	:	PRINT statement {$$ = new Prints($2);}
			| PRINTLN statement {$$ = new Prints($2, '\n');}
statement	: prints {$$ = new vector<Print*>;$$->push_back($1);}
					| statement ',' prints {$1->push_back($3);$$ = $1;}
prints	:	STRING {$$ = new Print($1);}
				| expr {$$ = new Print($1);}
expr	: term {$$ = $1;}
			| expr '+' expr {$$ = new BinExpr($1, $3, '+');}
			|	expr '*' expr {$$ = new BinExpr($1, $3, '*');}
			|	expr '-' expr {$$ = new BinExpr($1, $3, '-');}
			|	expr '/' expr {$$ = new BinExpr($1, $3, '/');}

boolexpr	: expr GT expr {$$ = new BoolExpr($1, $3, ">");}
					|	expr GTE expr {$$ = new BoolExpr($1, $3, ">=");}
					|	expr LT expr {$$ = new BoolExpr($1, $3, "<");}
					|	expr LTE expr {$$ = new BoolExpr($1, $3, "<=");}
					|	expr DE expr {$$ = new BoolExpr($1, $3, "==");}
					|	expr NE expr {$$ = new BoolExpr($1, $3, "!=");}
term	:	IDS {$$ = new Term($1);}
			| NUMBER {$$ = new Term($1);}
IDS	:	IDENTIFIER {$$ = new Id($1);}
		| IDENTIFIER '[' expr ']' {$$ = new Id($1, $3);}
MIDS : IDS {$$ = new vector<Id*>;$$->push_back($1);}
		| MIDS ',' IDS {$1->push_back($3);$$ = $1;}

%%

void yyerror (char const *s)
{
       fprintf (stderr, "%s\n", s);
       printf("THE ERROR IS AT: %d\n", x + 1);
}
int main(int argc, char** argv) {
	FILE *myfile = fopen(argv[1], "r");
	yyin = myfile;
	do {
		yyparse();
	} while (!feof(yyin));
	root->accept(visitor);
	return 0;
}