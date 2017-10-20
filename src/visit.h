#include <bits/stdc++.h>

using namespace std;

class visitor{

};
class interpreter : public visitor{
	map<Id, int, compare> table;
	void accept(Program* _node){
		_node->Decl_Statement->accept(), _node->Code_Statement_Block->accept();
	}
	void accept(Decl_Statement* _node){
		for(auto u : _node->ids)
			u->accept()
	}
	void accept(Code_Statement_Block* _node){
		for(auto u : _node->statements)
			u->accept();
	}
};