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
class Vstr{
public:
	virtual void accept(Program* _node)= 0;
	virtual void accept(Decl_Statement* _node)= 0;
	virtual void accept(Code_Statement_Block* _node)= 0;
	virtual void accept(Code_Statement* _node)= 0;
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
	void accept(Vstr* _v){
		_v->accept(this);
	}
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
	int accept(Vstr* _v){
		return _v->accept(this);
	}
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
class compare{
	public:
		bool operator()(const Id &lhs, const Id &rhs){
			return lhs.name < rhs.name;
		}
};
class interpreter : public Vstr{
	map<Id, int, compare> table;
public:
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
		for(auto u : *(_node->statements))
			u->accept(this);
	}
	void accept(Id* _node){
		if(_node->exp != NULL){
			int number = _node->exp->accept(this);
			for(int i = 0; i < number; i++)
				table[*(new Id(_node->name + "^" + to_string(i) + "^"))] = 0;
		}
		else
			table[*_node] = 0;
	}
	int accept(Term* _node){
		return ((_node->id == NULL) ? _node->val : table[eval(_node->id)]);
	}
	void accept(Code_Statement* _node){
		((dynamic_cast<Assignment*>(_node)) ? (dynamic_cast<Assignment *>(_node))->accept(this) : 
		((dynamic_cast<Forloop*>(_node)) ? (dynamic_cast<Forloop*>(_node))->accept(this) :
		((dynamic_cast<Whileloop*>(_node)) ? (dynamic_cast<Whileloop*>(_node))->accept(this) :
		((dynamic_cast<IfElse*>(_node)) ? (dynamic_cast<IfElse*>(_node))->accept(this) :
		((dynamic_cast<Prints*>(_node)) ? (dynamic_cast<Prints*>(_node))->accept(this) :
		(dynamic_cast<Read*>(_node))->accept(this))))));
	}
	void accept(Assignment* _node){
		table[eval(_node->id)] = _node->exp->accept(this);
	}
	void accept(Read* _node){
		for(auto u : *(_node->ids))
			cin >> table[eval(u)];
	}
	void accept(Whileloop* _node){
		for(;_node->boolexp->accept(this);)
			_node->block->accept(this);
	}
	void accept(IfElse* _node){
		if(_node->boolexp->accept(this))
			_node->ifblock->accept(this);
		else if(_node->elseblock != NULL)
			_node->elseblock->accept(this);
	}
	void accept(Forloop* _node){
		Id i = eval(_node->id);
		for(table[i] = _node->init; table[i] < _node->end; table[i] += _node->incr)
			_node->block->accept(this);
	}
	void accept(Print* _node){
		if(!_node->exp)
			cout << _node->str;
		else
			cout << _node->exp->accept(this);
	}
	void accept(Prints* _node){
		for(auto u : *(_node->print))
			u->accept(this);
	}
	int accept(Expr* _node){
		return ((dynamic_cast<BinExpr*>(_node)) ? (dynamic_cast<BinExpr*>(_node))->accept(this) :
		(((dynamic_cast<BoolExpr*>(_node)) ? (dynamic_cast<BoolExpr*>(_node))->accept(this) :
		(dynamic_cast<Term*>(_node))->accept(this))));
	}
	int accept(BinExpr* _node){
		cout << _node->l->accept(this) << " " << _node->r->accept(this) << '\n';
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
#endif
