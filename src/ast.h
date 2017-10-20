#ifndef _AST_H
#define _AST_H
#include <bits/stdc++.h>

using namespace std;

class Node{
public:
};
class Program;
class Decl_Statement;
class Code_Statement_Block;
class Code_Statement;
class Assignment;
class Id;
class Term;
class Expr;
class BinExpr;
class BoolExpr;
class Forloop;
class Whileloop;
class IfElse;
class Print;
class Prints;
class Read;
class GoTo;
class Vstr{
public:
	virtual void accept(Program* _node)= 0;
	virtual void accept(Decl_Statement* _node)= 0;
	virtual void accept(Code_Statement_Block* _node)= 0;
	virtual void accept(Assignment* _node)= 0;
	virtual void accept(Id* _node)= 0;
	virtual int accept(Expr* _node)= 0;
	virtual int accept(Term* _node)= 0;
	virtual int accept(BinExpr* _node)= 0;
	virtual int accept(BoolExpr* _node)= 0;
	virtual void accept(Forloop* _node)= 0;
	virtual void accept(Whileloop* _node)= 0;
	virtual void accept(IfElse* _node)= 0;
	virtual void accept(Print* _node)= 0;
	virtual void accept(Prints* _node)= 0;
	virtual void accept(Read* _node)= 0;
	virtual void accept(GoTo* _node)= 0;
	virtual Id eval(Id* _node)= 0;
};

class Program : public Node{
public:
	Decl_Statement *dstatement;
	Code_Statement_Block *cstatement;
	Program(Decl_Statement* _dstatement, Code_Statement_Block* _cstatement){
		dstatement = _dstatement, cstatement = _cstatement;
	}
	void accept(Vstr* _v){
		_v->accept(this);
	}
};
Program* root;
class Decl_Statement : public Node{
public:
	vector<Id*> *ids;
	Decl_Statement(vector<Id*>* _ids){
		ids = _ids;
	}
	void accept(Vstr* _v){
		_v->accept(this);
	}
};
class Code_Statement_Block : public Node{
public:
	vector<Code_Statement*> *statements;
	Code_Statement_Block(){
		this->statements = new vector<Code_Statement*>;
	}
	void add(Code_Statement* statement){
		statements->push_back(statement);
	}
	Code_Statement_Block(vector<Code_Statement*>* _statements){
		statements = _statements;
	}
	void accept(Vstr* _v){
		_v->accept(this);
	}
};
class Code_Statement : public Node{
public:
	virtual ~Code_Statement(){

	}
	virtual void accept(Vstr* _v) =0;
};
class Assignment : public Code_Statement{
public:
	Id *id;
	Expr *exp;
	Assignment(Id *_id, Expr *_exp){
		id = _id, exp = _exp; 
	}
	void accept(Vstr* _v){
		_v->accept(this);
	}
};
class Id : public Node{
public:
	string name;
	Expr* exp;
	Id(string _name, Expr* _exp = NULL){
		name = _name, exp = _exp;
	}
	void accept(Vstr* _v){
		_v->accept(this);
	}
};
class Expr : public Node{
public:
	virtual ~Expr(){
	}
	virtual int accept(Vstr* _v) =0;
};
class Term : public Expr{
public:
	int val;
	Id* id;
	Term(int _val){
		val = _val, id = NULL;
	}
	Term(Id* _id){
		id = _id;
	}
	int accept(Vstr* _v){
		return _v->accept(this);
	}
};
class BinExpr : public Expr{
public:
	Expr *l, *r;
	char operand;
	BinExpr(Expr *_l, Expr *_r, char _operand){
		l = _l, r = _r, operand = _operand; 
	}
	int accept(Vstr* _v){
		return _v->accept(this);
	}
};
class BoolExpr : public Expr{
public:
	Expr *l, *r;
	string operand;
	BoolExpr(Expr *_l, Expr *_r, string _operand){
		l = _l, r = _r, operand = _operand;
	}
	int accept(Vstr* _v){
		return _v->accept(this);
	}
};
class Forloop : public Code_Statement{
public:
	Id* id;
	Code_Statement_Block* block;
	int init, incr, end;
	Forloop(Id* _id, int _init, int _end, Code_Statement_Block* _block, int _incr = 1){
		id = _id, init = init, incr = _incr, end = _end, block = _block;
	}
	void accept(Vstr* _v){
		_v->accept(this);
	}
};
class Whileloop : public Code_Statement{
public:
	BoolExpr* boolexp;
	Code_Statement_Block* block;
	Whileloop(BoolExpr* _boolexp, Code_Statement_Block* _block){
		boolexp = _boolexp, block = _block;
	}
	void accept(Vstr* _v){
		_v->accept(this);
	}
};
class IfElse : public Code_Statement{
public:
	BoolExpr* boolexp;
	Code_Statement_Block* ifblock;
	Code_Statement_Block* elseblock;
	IfElse(BoolExpr* _boolexp, Code_Statement_Block* _ifblock, Code_Statement_Block* _elseblock){
		boolexp = _boolexp, ifblock = _ifblock, elseblock = _elseblock;
	}
	IfElse(BoolExpr* _boolexp, Code_Statement_Block* _ifblock){
		boolexp = _boolexp, ifblock = _ifblock;
	}
	void accept(Vstr* _v){
		_v->accept(this);
	}
};
class Print : public Node{
public:
	string str;
	Expr* exp;
	Print(string _str){
		str = _str;
	}
	Print(Expr* _exp){
		exp = _exp;
	}
	void accept(Vstr* _v){
		_v->accept(this);
	}
};
class Prints : public Code_Statement{
public:
	vector<Print*> *print;
	char end = '\0';
	Prints(vector<Print*>* _print){
		print = _print;
	}
	Prints(vector<Print*>* _print, char _end){
		print = _print, end = _end;
	}
	void accept(Vstr* _v){
		_v->accept(this);
	}
};

class Read : public Code_Statement{
public:
	vector<Id*> *ids;
	Read(vector<Id*> *_ids){
		ids = _ids;
	}
	void accept(Vstr* _v){
		_v->accept(this);
	}
};
class GoTo : public Code_Statement{
public:
	string id;
	BoolExpr* exp = NULL;
	GoTo(string _id){
		id = _id;
	}
	GoTo(string _id, BoolExpr* _exp){
		id = _id, exp = _exp;
	}
	void accept(Vstr* _v){
		_v->accept(this);
	}
};
class compare{
	public:
		bool operator()(const Id &lhs, const Id &rhs){
			return lhs.name < rhs.name;
		}
};
class interpreter : public Vstr{
public:
	map<Id, int, compare> table;
	map<string, vector<Code_Statement*> > label_table;
	Id eval(Id* _node){
		if(_node->exp == NULL)return *_node;
		int number = _node->exp->accept(this);
		if(table.find(*(new Id(_node->name + "^" + to_string(number) + "^"))) == table.end())exit(-1);
		return *(new Id(_node->name + "^" + to_string(number) + "^"));
	}
	void accept(Program* _node){
		_node->dstatement->accept(this), _node->cstatement->accept(this);
	}
	void accept(Decl_Statement* _node){
		for(auto u : *(_node->ids))
			u->accept(this);
	}
	void accept(Code_Statement_Block* _node){
		// cout << "CBLOCK" << '\n';
		for(auto u : *(_node->statements))
			u->accept(this);
	}
	void accept(Id* _node){
		// cout << "ID" << '\n';
		if(_node->exp != NULL){
			int number = _node->exp->accept(this);
			for(int i = 0; i < number; i++)
				table[*(new Id(_node->name + "^" + to_string(i) + "^"))] = 0;
		}
		else
			table[*_node] = 0;
	}
	int accept(Term* _node){
		// cout << "TERM" << '\n';
		return ((_node->id == NULL) ? _node->val : table[eval(_node->id)]);
	}
	// void accept(Code_Statement* _node){
	// 	if((dynamic_cast<GoTo*>(_node)))
	// 		accept((dynamic_cast<GoTo*>(_node)));
	// 	((dynamic_cast<Assignment*>(_node)) ? (dynamic_cast<Assignment *>(_node))->accept(this) : 
	// 	((dynamic_cast<Forloop*>(_node)) ? (dynamic_cast<Forloop*>(_node))->accept(this) :
	// 	((dynamic_cast<Whileloop*>(_node)) ? (dynamic_cast<Whileloop*>(_node))->accept(this) :
	// 	((dynamic_cast<IfElse*>(_node)) ? (dynamic_cast<IfElse*>(_node))->accept(this) :
	// 	((dynamic_cast<Prints*>(_node)) ? (dynamic_cast<Prints*>(_node))->accept(this) :
	// 	((dynamic_cast<GoTo*>(_node)) ? (dynamic_cast<GoTo*>(_node))->accept(this) :
	// 	(dynamic_cast<Read*>(_node))->accept(this)))))));
	// }
	void accept(Assignment* _node){
		// cout << "Assignment" << '\n';
		table[eval(_node->id)] = _node->exp->accept(this);
	}
	void accept(Read* _node){
		// cout << "Read" << '\n';
		for(auto u : *(_node->ids))
			cin >> table[eval(u)];
	}
	void accept(Whileloop* _node){
		// cout << "WHILE" << '\n';
		for(;_node->boolexp->accept(this);)
			_node->block->accept(this);
	}
	void accept(IfElse* _node){
		// cout << "IF" << '\n';
		if(_node->boolexp->accept(this))
			_node->ifblock->accept(this);
		else if(_node->elseblock != NULL)
			_node->elseblock->accept(this);
	}
	void accept(Forloop* _node){
		// cout << "FOR" << '\n';
		Id i = eval(_node->id);
		for(table[i] = _node->init; table[i] < _node->end; table[i] += _node->incr)
			_node->block->accept(this);
	}
	void accept(Print* _node){
		// cout << "PRINT" << '\n';
		if(!_node->exp)
			cout << _node->str;
		else
			cout << _node->exp->accept(this);
	}
	void accept(Prints* _node){
		// cout << "PRINTS" << '\n';
		for(auto u : *(_node->print))
			u->accept(this);
		cout << _node->end;
	}
	void accept(GoTo* _node){
		// if(label_table.find(_node->id) == label_table.end() || table.find(*(new Id(_node->id))) != table.end())
		// if(label_table.find(_node->id) == label_table.end() || table.find(*(new Id(_node->id))) != table.end())
		// 	exit(-1);
		if(_node->exp == NULL || _node->exp->accept(this)){
			for(auto u : label_table[_node->id])
				u->accept(this);
			exit(0);
		}
	}
	int accept(Expr* _node){
		// cout << "EXPR" << '\n';
		return ((dynamic_cast<BinExpr*>(_node)) ? (dynamic_cast<BinExpr*>(_node))->accept(this) :
		(((dynamic_cast<BoolExpr*>(_node)) ? (dynamic_cast<BoolExpr*>(_node))->accept(this) :
		(dynamic_cast<Term*>(_node))->accept(this))));
	}
	int accept(BinExpr* _node){
		// cout << "BINEXPR" << '\n';
		// cout << _node->l->accept(this) << " " << _node->r->accept(this) << '\n';
		if(_node->operand == '+')
			return _node->l->accept(this) + _node->r->accept(this);
		if(_node->operand == '-')
			return _node->l->accept(this) - _node->r->accept(this);
		if(_node->operand == '*')
			return _node->l->accept(this) * _node->r->accept(this);
		if(_node->operand == '/')
			return _node->l->accept(this) / _node->r->accept(this);
	}
	int accept(BoolExpr* _node){
		// cout << "BoolExpr" << _node->l->accept(this) << " " << _node->r->accept(this) << '\n';
		if(_node->operand == ">")
			return _node->l->accept(this) > _node->r->accept(this);
		if(_node->operand == ">=")
			return _node->l->accept(this) >= _node->r->accept(this);
		if(_node->operand == "<")
			return _node->l->accept(this) < _node->r->accept(this);
		if(_node->operand == "<=")
			return _node->l->accept(this) <= _node->r->accept(this);
		if(_node->operand == "==")
			return _node->l->accept(this) == _node->r->accept(this);
		if(_node->operand == "!=")
			return _node->l->accept(this) != _node->r->accept(this);
	}
};
interpreter* visitor = new interpreter();
#endif
