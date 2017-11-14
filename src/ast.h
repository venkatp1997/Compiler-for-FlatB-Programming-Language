#ifndef _AST_H
#define _AST_H
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Bitcode/BitstreamReader.h>
#include <llvm/Bitcode/BitstreamWriter.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>
#include <bits/stdc++.h>

using namespace std;
using namespace llvm;

class Node{
public:
  virtual Value *Codegen() = 0;
};
class Program;
class Decl_Statement;
class Code_Statement_Block;
class Code_Statement;
class Assignment;
class Term;
class NumTerm;
class VarTerm;
class Expr;
class Id;
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
	virtual int accept(NumTerm* _node)= 0;
	virtual int accept(VarTerm* _node)= 0;
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
vector<Type*> argTypes;
FunctionType *ftype = FunctionType::get(Type::getVoidTy(getGlobalContext()), makeArrayRef(argTypes), false);
Module *module = new Module("main", getGlobalContext());
Function *func_main = Function::Create(ftype, GlobalValue::InternalLinkage, "main", module);
BasicBlock *block_main = BasicBlock::Create(getGlobalContext(), "entry", func_main, 0);
stack<Code_Statement_Block*> blocks;
IRBuilder<> Builder(block_main);
map<std::string, Value*> NamedValues;
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
  virtual Value *Codegen();
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
  virtual Value *Codegen();
};
class Code_Statement_Block : public Node{
public:
	vector<Code_Statement*> *statements;
  BasicBlock *block;
  Value *ret;
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
  virtual Value *Codegen();
};
class Code_Statement : public Node{
public:
	virtual ~Code_Statement(){

	}
	virtual void accept(Vstr* _v) =0;
  virtual Value *Codegen() = 0;
};
Value* Program::Codegen(){
  Value* last = NULL;
  last = this->dstatement->Codegen();
  last = this->cstatement->Codegen();
  return last;
}
Value* Code_Statement_Block::Codegen(){
  Value* last = NULL;
  for(auto u : *(this->statements))
    last = u->Codegen();
  return last;
}
void pushBlock(BasicBlock *block){
   blocks.push(new Code_Statement_Block());
   blocks.top()->ret = NULL;
   blocks.top()->block = block; 
}
void popBlock(){
  /* Code_Statement_Block *tmp = blocks.top(); */
  blocks.pop();
  /* delete tmp; */
}
class Expr : public Node{
public:
	virtual ~Expr(){
	}
	virtual int accept(Vstr* _v) =0;
  virtual Value *Codegen() = 0;
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
  virtual Value* Codegen();
};
Value* Decl_Statement::Codegen(){
  Value* last = NULL;
  for(auto u : *(this->ids))
    last = u->Codegen();
  return last;
}
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
  virtual Value *Codegen(){
    cout << "Assignment for " << id->name << '\n';
    return new StoreInst(exp->Codegen(), NamedValues[id->name], false, blocks.top()->block);
  }
};
Value* Id::Codegen(){
  cout << "Declaration for " << name << '\n';
  AllocaInst *alloc = new AllocaInst(IntegerType::get(getGlobalContext(), 32), name.c_str(), blocks.top()->block);
  NamedValues[name] = alloc;
  if(exp != NULL){
    Assignment* assn = new Assignment(new Id(name), exp);
    cout << name << '\n';
    assn->Codegen();
  }
  return alloc;
}
class NumTerm : public Expr{
  public:
    int val;
    NumTerm(int _val){
      val = _val;
    }
    int accept(Vstr* _v){
      return _v->accept(this);
    }
    virtual Value *Codegen(){
      return ConstantInt::get(Type::getInt32Ty(getGlobalContext()), val, true);
    }
};
class VarTerm : public Expr{
  public:
    Id* id;
    VarTerm(Id* _id){
      id = _id;
    }
    int accept(Vstr* _v){
      return _v->accept(this);
    }
    virtual Value *Codegen(){
      /* if(NamedValues.find(id->name) == NamedValues.end()) */
      /*   NamedValues[id->name] = ConstantInt::get(Type::getInt64Ty(getGlobalContext()), 0, true); */
      /* cout << (valA == NULL) << '\n'; */
      return NamedValues[id->name];
    }
};
class Term : public Expr{
public:
  NumTerm* num;
  VarTerm* var;
	Term(int _val){
		num = new NumTerm(_val), var = NULL;
	}
	Term(Id* _id){
		var = new VarTerm(_id), num = NULL;
	}
	int accept(Vstr* _v){
		return _v->accept(this);
	}
  virtual Value *Codegen(){
    if(this->var == NULL)
      return num->Codegen();
    else
      return var->Codegen();
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
  virtual Value *Codegen(){
    Value *L = l->Codegen();
    Value *R = r->Codegen();
    switch(operand){
      case '+': return Builder.CreateFAdd(L, R, "addtmp");
      case '-': return Builder.CreateFSub(L, R, "subtmp");
      case '*': return Builder.CreateFMul(L, R, "multmp");
      case '/': return Builder.CreateFDiv(L, R, "divtmp");
    }
  }
};
class BoolExpr : public Expr{
public:
	Expr *l, *r;
	string operand;
  Value* L1;
  Value* R1;
	BoolExpr(Expr *_l, Expr *_r, string _operand){
		l = _l, r = _r, operand = _operand;
    L1 = NULL, R1 = NULL;
	}
	BoolExpr(Value *_l, Value *_r, string _operand){
    l = NULL;
    r = NULL;
    L1 = _l, R1 = _r;
  }
	int accept(Vstr* _v){
		return _v->accept(this);
	}
  virtual Value *Codegen(){
    Value *L;
    Value *R;
    if(L1 == NULL && R1 == NULL){
      Value *L = l->Codegen();
      Value *R = r->Codegen();
    }
    else
      L = L1, R = R1;
    if(L == 0 || R == 0)return 0;
    if(operand == "<")L = Builder.CreateFCmpULT(L, R, "cmpltmp");
    if(operand == "<=")L = Builder.CreateFCmpULE(L, R, "cmpletmp");
    if(operand == ">")L = Builder.CreateFCmpUGT(L, R, "cmpgtmp");
    if(operand == ">=")L = Builder.CreateFCmpUGE(L, R, "cmpgetmp");
    if(operand == "==")L = Builder.CreateFCmpUEQ(L, R, "cmpeqtmp");
    if(operand == "!=")L = Builder.CreateFCmpUNE(L, R, "cmpnetmp");
    return Builder.CreateUIToFP(L, Type::getDoubleTy(getGlobalContext()), "booltmp");
  }
};
class Forloop : public Code_Statement{
public:
	Id* id;
	Code_Statement_Block* block;
  int init, incr, last;
  BoolExpr* end;
	Forloop(Id* _id, int _init, int _last, Code_Statement_Block* _block, int _incr = 1){
		id = _id, init = _init, incr = _incr, last = _last, block = _block;
	}
	void accept(Vstr* _v){
		_v->accept(this);
	}
  virtual Value* Codegen() {
    // Emit the start code first, without 'variable' in scope.
    Assignment* assn = new Assignment(id, new NumTerm(init));
    assn->Codegen();
    /* if (!StartVal) */
    /*   return nullptr; */

    /* // Make the new basic block for the loop header, inserting after current */
    /* // block. */
    Function *TheFunction = func_main;
    BasicBlock *PreheaderBB = Builder.GetInsertBlock();
    BasicBlock *LoopBB = BasicBlock::Create(getGlobalContext(), "loop", TheFunction);

    /* // Insert an explicit fall through from the current block to the LoopBB. */
    Builder.CreateBr(LoopBB);

    /* // Start insertion in LoopBB. */
    Builder.SetInsertPoint(LoopBB);
    pushBlock(LoopBB);

    /* // Start the PHI node with an entry for Start. */
    /* PHINode *Variable = */
    /*   Builder.CreatePHI(Type::getDoubleTy(TheContext), 2, VarName); */
    /* Variable->addIncoming(StartVal, PreheaderBB); */

    /* // Within the loop, the variable is defined equal to the PHI node.  If it */
    /* // shadows an existing variable, we have to restore it, so save it now. */
    /* Value *OldVal = NamedValues[id->name]; */
    /* NamedValues[id->name] = ; */

    /* // Emit the body of the loop.  This, like any other expr, can change the */
    /* // current BB.  Note that we ignore the value computed by the body, but don't */
    /* // allow an error. */
    block->Codegen();
    /* if (!Body->codegen()) */
    /*   return nullptr; */

    /* // Emit the step value. */
    /* Value *StepVal = nullptr; */
    /* if (Step) { */
    /*   StepVal = Step->codegen(); */
    /*   if (!StepVal) */
    /*     return nullptr; */
    /* } else { */
    /*   // If not specified, use 1.0. */
    /*   StepVal = ConstantFP::get(TheContext, APFloat(1.0)); */
    /* } */
    Value *StepVal = (new NumTerm(incr))->Codegen();
	  Value *NextVar = BinaryOperator::Create(Instruction::Add, NamedValues[id->name], StepVal, "nextvar", blocks.top()->block);
    NamedValues[id->name] = NextVar;
    Value* EndVal = (new NumTerm(last))->Codegen();
    /* // Compute the end condition. */
    BoolExpr* endExpr = new BoolExpr(NamedValues[id->name], EndVal, "<");
    Value *EndCond = endExpr->Codegen();
    /* if (!EndCond) */
    /*   return nullptr; */

    /* // Convert condition to a bool by comparing non-equal to 0.0. */
		FCmpInst *comparison = new FCmpInst(*blocks.top()->block, FCmpInst::FCMP_UNE, EndCond, ConstantFP::get(getGlobalContext(), APFloat(0.0)), "cmp");
    popBlock();
    /* EndCond = Builder.CreateFCmpONE( */
    /*     EndCond, ConstantFP::get(getGlobalContext(), APFloat(0.0)), "loopcond"); */

    /* // Create the "after loop" block and insert it. */
    BasicBlock *LoopEndBB = Builder.GetInsertBlock();
    BasicBlock *AfterBB =
      BasicBlock::Create(getGlobalContext(), "afterloop", TheFunction);

    /* // Insert the conditional branch into the end of LoopEndBB. */
    Builder.CreateCondBr(comparison, LoopBB, AfterBB);

    /* // Any new code will be inserted in AfterBB. */
    Builder.SetInsertPoint(AfterBB);
    pushBlock(AfterBB);

    /* // Add a new entry to the PHI node for the backedge. */
    /* Variable->addIncoming(NextVar, LoopEndBB); */

    /* // Restore the unshadowed variable. */
    /* if (OldVal) */
    /*   NamedValues[VarName] = OldVal; */
    /* else */
    /*   NamedValues.erase(VarName); */

    /* // for expr always returns 0.0. */
    return Constant::getNullValue(Type::getDoubleTy(getGlobalContext()));
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
  virtual Value *Codegen(){
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
  virtual Value *Codegen(){
    Value *CondV = boolexp->Codegen();
    if (CondV == 0)
      return 0;

    // Convert condition to a bool by comparing equal to 0.0.
    CondV = Builder.CreateFCmpONE(
        CondV, ConstantFP::get(getGlobalContext(), APFloat(0.0)), "ifcond");

    Function *TheFunction = func_main;

    // Create blocks for the then and else cases.  Insert the 'then' block at the
    // end of the function.
    BasicBlock *ThenBB =
        BasicBlock::Create(getGlobalContext(), "then", TheFunction);
    BasicBlock *ElseBB = BasicBlock::Create(getGlobalContext(), "else");
    BasicBlock *MergeBB = BasicBlock::Create(getGlobalContext(), "ifcont");
    Builder.CreateCondBr(CondV, ThenBB, ElseBB);
    /* cout << "CreateCondBr " << ret << '\n'; */
    // Emit then value.
    Builder.SetInsertPoint(ThenBB);
    pushBlock(ThenBB);
    Value *ThenV = ifblock->Codegen();
    popBlock();
    if (ThenV == 0)
      return 0;

    Builder.CreateBr(MergeBB);
    /* cout << "CreateBr1" << ret << '\n'; */
    // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
    ThenBB = Builder.GetInsertBlock();

    // Emit else block.
    TheFunction->getBasicBlockList().push_back(ElseBB);
    Builder.SetInsertPoint(ElseBB);

    pushBlock(ElseBB);
    Value *ElseV = elseblock->Codegen();
    popBlock();
    if (ElseV == 0)
      return 0;

    Builder.CreateBr(MergeBB);
    /* cout << "CreateBr2" << ret << '\n'; */
    // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
    ElseBB = Builder.GetInsertBlock();

    // Emit merge block.
    pushBlock(MergeBB);
    TheFunction->getBasicBlockList().push_back(MergeBB);
    Builder.SetInsertPoint(MergeBB);
    /* PHINode *PN = */
    /*     Builder.CreatePHI(Type::getDoubleTy(getGlobalContext()), 2, "iftmp"); */
    /* PN->addIncoming(ThenV, ThenBB); */
    /* PN->addIncoming(ElseV, ElseBB); */
    return Constant::getNullValue(Type::getDoubleTy(getGlobalContext()));
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
  virtual Value *Codegen(){
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
  virtual Value *Codegen(){
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
  virtual Value *Codegen(){
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
  virtual Value *Codegen(){
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
		return ((_node->var == NULL) ? _node->num->accept(this) : _node->var->accept(this));
	}
	int accept(NumTerm* _node){
		return _node->val;
	}
	int accept(VarTerm* _node){
		return table[eval(_node->id)];
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
    return;
		/* Id i = eval(_node->id); */
		/* for(table[i] = _node->init; table[i] < _node->end; table[i] += _node->incr) */
		/* 	_node->block->accept(this); */
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
		cout << _node->end;
	}
	void accept(GoTo* _node){
		if(_node->exp == NULL || _node->exp->accept(this)){
			for(auto u : label_table[_node->id])
				u->accept(this);
			exit(0);
		}
	}
	int accept(Expr* _node){
		return ((dynamic_cast<BinExpr*>(_node)) ? (dynamic_cast<BinExpr*>(_node))->accept(this) :
		(((dynamic_cast<BoolExpr*>(_node)) ? (dynamic_cast<BoolExpr*>(_node))->accept(this) :
		(dynamic_cast<Term*>(_node))->accept(this))));
	}
	int accept(BinExpr* _node){
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
interpreter* visitor = new interpreter();
#endif
