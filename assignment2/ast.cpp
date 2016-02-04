#include <iostream>
#include "ast.h"
#include <string>
using namespace std;

string get_enum[] = {
	"",
	"OR",
	"AND",
	"EQ_OP",
	"NE_OP",
	"LT",
	"LE_OP",
	"GE_OP",
	"PLUS",
	"MINUS",
	"MULT",
	"ASSIGN",
	"UMINUS",
	"NOT",
	"PP"
};


////////////////////////////////////////////////////////

BlockStatement::BlockStatement(){}
BlockStatement::BlockStatement(StmtAst* c){
	children.push_back(c);
}
void BlockStatement::print(){
	if (children.empty()) cout << "(Empty)";
	cout << "(Block [";
	for (int i = 0; i < children.size(); i++) {
		children.at(i)->print();
		if (i < children.size() - 1) cout << " ";
	}
	cout << "])";
}

void Empty::print(){
	cout << "(Empty)";
}


//Seq

Seq::Seq(){}
Seq::Seq(StmtAst* c){
	child = c;
}
void Seq::print(){
	cout << "(Seq ";
	child -> print();
	cout << ")";
}

//Assignment

Ass::Ass(){
	empty = true;
}
Ass::Ass(ExpAst* l, ExpAst* r){
	left = l;
	right = r;
	empty = false;
}
void Ass::print(){
	if(empty){
		cout << "(Empty)";
		return;
	}
	cout << "(Ass ";
	left-> print();
	cout << " ";
	right-> print();
	cout << ")";
}


//Return
Return::Return(){}
Return::Return(ExpAst* c) {
	child = c;
}

void Return::print(){
	cout << "(Return ";
	child->print();
	cout << ")";
}

//If

If::If(){}
If::If(ExpAst* f, StmtAst* s, StmtAst* t){
	first = f;
	second = s;
	third = t;
}

void If::print(){
	cout << "(If ";
	first->print();
	cout << " ";
	second->print();
	cout << " ";
	third->print();
	cout << ")";
}
//While

While::While() {}
While::While(ExpAst* l, StmtAst* r){
	left = l;
	right = r;
}

void While::print(){
	cout << "(While ";
	left->print();
	cout << " ";
	right->print();
	cout << ")";
}

//For

For::For(){}
For::For(ExpAst* f, ExpAst* s, ExpAst* t, StmtAst* c){
	first = f;
	second = s;
	third = t;
	child = c;
}

void For::print(){
	cout << "(For ";
	first->print();
	cout << " ";
	second->print();
	cout << " ";
	third->print();
	cout << " ";
	child->print();
	cout << ")";
}

////////////////////////////////////////////////////////

OpBinary::OpBinary(){};
OpBinary::OpBinary(opNameB op){
	opName = op;
}
void OpBinary::setArguments(ExpAst* x, ExpAst *y){
	left = x;
	right = y;
}
OpBinary::OpBinary(ExpAst*x, ExpAst*y, opNameB op){
	left = x;
	right = y;
	opName = op;
}

void OpBinary::print(){
	cout<<"("<< get_enum[opName] << " ";
  	left->print();
  	cout<<" ";
  	right->print();
  	cout<<")";
}

//Unary operators

OpUnary::OpUnary(){}
OpUnary::OpUnary(opNameU op){
	opName = op;
}
OpUnary::OpUnary(ExpAst* x, opNameU op) {
	child = x;
	opName = op;
}
OpUnary::OpUnary(ExpAst * x, OpUnary* y) {
	opName = y->opName;
	child = x;
}
void OpUnary::print(){
	cout<<"("<< get_enum[opName] << " ";
  	child->print();
  	cout<<")";
}

//Function calls

Funcall::Funcall(){}
Funcall::Funcall(std::vector <ExpAst*> expressions){
	children = expressions;
}
Funcall::Funcall(ExpAst* x){
	children.push_back(x);
}
void Funcall::print(){
	cout << "(Funcall ";
	for(int i = 0; i < children.size(); i++){
    children[i]->print();
    if (i < (children.size() - 1)) cout << " ";
    }
  	cout << ")";
}

//Float Constants

FloatConst::FloatConst(float x){
	child = x;
}
void FloatConst::print(){
	cout << "(FloatConst "<< child << ")";
}

//Int Constants

IntConst::IntConst(){}
IntConst::IntConst(int x){
	child = x;
}
void IntConst::print(){
	cout << "(IntConst " << child << ")";
}

//String Constants

StringConst::StringConst(string x){
	child = x;
}
void StringConst::print(){
	cout << "(StringConst " << child << ")";
}




////////////////////////////////////////////////////////

Identifier::Identifier(){}
Identifier::Identifier(string x){
	child = x;
}
void Identifier::print(){
	cout<<"(Identifier \"" << child << "\")";
}


//Array

ArrayRef::ArrayRef(){}
ArrayRef::ArrayRef(Identifier* left, ExpAst* right){
  this->left = left;
  this->right = right;
}
void ArrayRef::print(){
  cout<<"(Array ";
  left->print();
  cout << " ";
  right->print();
  cout << ")";
}


//Pointer

Pointer::Pointer(){}
Pointer::Pointer(RefAst *child){
	this->child =child;
}
void Pointer::print(){
	cout << "(Pointer";
	child->print();
	cout <<")";
}


//De reference

DeRef::DeRef(){}
DeRef::DeRef(RefAst *child){
	this->child =child;
}
void DeRef::print(){
	cout << "(Dereference";
	child->print();
	cout <<")";
}