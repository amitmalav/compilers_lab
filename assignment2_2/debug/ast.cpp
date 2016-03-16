#include <iostream>
#include <string>
using namespace std;

string get_enum[] = {
	"",
	"OR",
	"AND",
	"EQ_OP",
	"EQ_OP_INT",
	"EQ_OP_FLOAT",
	"NE_OP",
	"NE_OP_INT",
	"NE_OP_FLOAT",
	"LT",
	"LT_INT",
	"LT_FLOAT",
	"GT",
	"GT_INT",
	"GT_FLOAT",
	"LE_OP",
	"LE_OP_INT",
	"LE_OP_FLOAT",
	"GE_OP",
	"GE_OP_INT",
	"GE_OP_FLOAT",
	"PLUS",
	"PLUS_INT",
	"PLUS_FLOAT",
	"MINUS",
	"MINUS_INT",
	"MINUS_FLOAT",
	"MULT",
	"MULT_INT",
	"MULT_FLOAT",
	"DIV",
	"DIV_INT",
	"DIV_FLOAT",
	"PTR_OP",
	"UMINUS",
	"NOT",
	"PP",
	"DEREF",
	"POINTER",
	"TO_FLOAT",
	"TO_INT"
};


////////////////////////////////////////////////////////

BlockStmt::BlockStmt(){}
BlockStmt::BlockStmt(StmtAst* c){
	children.push_back(c);
}
void BlockStmt::print(){
	cout << "(Block [";
	if (children.empty()) cout << "(Empty)";
	for (int i = 0; i < children.size(); i++) {
		children.at(i)->print();
		if (i < children.size() - 1) cout << " ";
	}
	cout << "])";
}

void Empty::print(){
	// cout << "(Empty)";
}


//Seq

Seq::Seq(){}
Seq::Seq(std::vector<StmtAst*> c){
	children = c;
}
Seq::Seq(StmtAst* c){
	children.push_back(c);
}
void Seq::insert_Seq(StmtAst* c){
	children.push_back(c);
}
void Seq::print(){
	cout << "(Seq ";
	for (int i = 0; i < children.size(); i++) {
		children.at(i)->print();
		if (i < children.size() - 1) cout << " ";
	}
	cout << ")";
}

//Assignment

Ass::Ass(){
	empty = true;
	type = new Type(Ok);
}
Ass::Ass(ExpAst* c){
	child = c;
	empty = false;
	type = c->type;
}
void Ass::print(){
	if(empty){
		// cout << "(Empty)";
		return;
	}
	// cout << "(Ass ";
	child-> print();
	// cout << ")";
}


//Return
Return::Return(){}
Return::Return(ExpAst* c, Type *t) {
	if(t->equal(c->type)){
		child = c;
		type = new Type(Ok);
	}
	else if (c->type->typeKind != Base) {
		type = new Type(Error);
		child = c;
	}
	else if ((c->type->base == Int) && (t->base == Float)){
		OpUnary *xf = new OpUnary(c, TO_FLOAT);
		child = xf;
		type = new Type(Ok);
	}
	else if ((c->type->base == Float) && (t->base == Int)){
		OpUnary *xf = new OpUnary(c, TO_INT);
		child = xf;
		type = new Type(Ok);
	}
	else{
		child = c;
		type = new Type(Error);
	}
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
OpBinary::OpBinary(ExpAst*x, ExpAst*y, opNameB op){

	opName = op;
	if(op == OR || op == AND){
		if((x->type->typeKind == Pointer) && (y->type->typeKind == Pointer)){
			left = x;
			right = y;
			type = new Type(Kind::Base, Basetype::Int);
			return;
		}
		if((x->type->typeKind == Pointer) && (y->type->typeKind != Pointer)){
			if(y->type->base == Struct){
				left = x;
				right = y;
				type = new Type(Error);
				return;
			}
			else{
				left = x;
				right = y;
				type = new Type(Kind::Base, Basetype::Int);
				return;
			}
		}
		if((x->type->typeKind != Pointer) && (y->type->typeKind == Pointer)){
			if(x->type->base == Struct){
				left = x;
				right = y;
				type = new Type(Error);
				return;
			}
			else{
				left = x;
				right = y;
				type = new Type(Kind::Base, Basetype::Int);
				return;
			}
		}
		if ((x->type->base == Int) && (y->type->base == Float)){
			OpUnary *xf = new OpUnary(x, TO_FLOAT);
			left = xf;
			right = y;
			type = new Type(Kind::Base, Basetype::Int);
			return;
		}
		if ((x->type->base == Float) && (y->type->base == Int)){
			OpUnary *xf = new OpUnary(y, TO_FLOAT);
			left = x;
			right = xf;
			type = new Type(Kind::Base, Basetype::Int);
			return;
		}
		if ((x->type->base == Int) && (y->type->base == Int)){
			left = x;
			right = y;
			type = new Type(Kind::Base, Basetype::Int);
			return;
		}
		if ((x->type->base == Float) && (y->type->base == Float)){
			left = x;
			right = y;
			type = new Type(Kind::Base, Basetype::Int);
			return;
		}

		else{
			left = x;
			right = y;
			type = new Type(Error);
			return;
		}

	}
	if(op == EQ_OP || op == NE_OP || op == LT || op == GT || op == LE_OP || op == GE_OP){
		if((x->type->typeKind == Pointer) && (y->type->typeKind == Pointer)){
			if(x->type->getType() == y->type->getType()){
				if(x->type->num_type_pointers == y->type->num_type_pointers){
					left = x;
					right = y;
					type = new Type(Kind::Base, Basetype::Int);
					return;
				}
			}
			left = x;
			right = y;
			type = new Type(Error);
			return;
		}

		if ((x->type->base == Int) && (y->type->base == Float)){
			OpUnary *xf = new OpUnary(x, TO_FLOAT);
			opName = (opNameB)((int)op+2);
			left = xf;
			right = y;
			type = new Type(Kind::Base, Basetype::Int);
			return;
		}
		if ((x->type->base == Float) && (y->type->base == Int)){
			OpUnary *xf = new OpUnary(y, TO_FLOAT);
			opName = (opNameB)((int)op+2);
			left = x;
			right = xf;
			type = new Type(Kind::Base, Basetype::Int);
			return;
		}
		if ((x->type->base == Int) && (y->type->base == Int)){
			opName = (opNameB)((int)op+1);
			left = x;
			right = y;
			type = new Type(Kind::Base, Basetype::Int);
			return;
		}
		if ((x->type->base == Float) && (y->type->base == Float)){
			opName = (opNameB)((int)op+2);
			left = x;
			right = y;
			type = new Type(Kind::Base, Basetype::Int);
			return;
		}

		else{
			left = x;
			right = y;
			type = new Type(Error);
			return;
		}

	}
	if(op == PLUS || op== MINUS || op== MULT || op== DIV){
		if((x->type->typeKind == Base) && (y->type->typeKind == Base)){
			if ((x->type->base == Int) && (y->type->base == Float)){
				OpUnary *xf = new OpUnary(x, TO_FLOAT);
				opName = (opNameB)((int)op+2);
				left = xf;
				right = y;
				type = y->type;
				return;
			}
			if ((x->type->base == Float) && (y->type->base == Int)){
				OpUnary *xf = new OpUnary(y, TO_FLOAT);
				opName = (opNameB)((int)op+2);
				left = x;
				right = xf;
				type = x->type;
				return;
			}
			if ((x->type->base == Int) && (y->type->base == Int)){
				opName = (opNameB)((int)op+1);
				left = x;
				right = y;
				type = x->type;
				return;
			}
			if ((x->type->base == Float) && (y->type->base == Float)){
				opName = (opNameB)((int)op+2);
				left = x;
				right = y;
				type = x->type;
				return;
			}
		}
		left = x;
		right = y;
		type = new Type(Error);
		return;
	}












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
	type = new Type(Ok);
	if(op == UMINUS || op == NOT){
		type->check = 3;
	}
	if(op == DEREF){
		type->check = 1;
	}
	if(op == POINTER){
		type->check = 2;
	}
	
}
OpUnary::OpUnary(ExpAst* x, opNameU op) {
	child = x;
	type = x->type;
	opName = op;
}
OpUnary::OpUnary(ExpAst * x, OpUnary* y) {
	opName = y->opName;
	if((y->opName == UMINUS) || (y->opName == NOT) || (y->opName == PP)){
		child = x;
		type = x->type;
		type->check = 3;
		return;
	}
	if(y->opName == POINTER){
		if(x->type->typeKind != Pointer){
			child = x;
			type = new Type(Error);
			return;
		}
		type = x->type->copy();
		type->check = 2;
		if(type->num_type_pointers == 1){
			if(type->base == Void){
				child = x;
				type = new Type(Error);
				return;
			}
			type->typeKind = Base;
			child = x;
			return;
		}
		else{
			type->num_type_pointers--;
			child = x;
			return;
		}

	}
	if(y->opName == DEREF){
		child = x;
		type = x->type->copy();
		type->check = 1;
		type->num_type_pointers += 1;
		if(x->type->typeKind != Pointer)x->type->typeKind = Pointer;
		return;
	}

}
void OpUnary::print(){
	cout<<"("<< get_enum[opName] << " ";
  	child->print();
  	cout<<")";
}

//Assign
Assign::Assign() {}
Assign::Assign(ExpAst* l, ExpAst* r){
	if((l->type)->equal(r->type)){
		left = l;
		right = r;
		type = l->type;
	}

	else if (!((l->type->typeKind == Base) &&(r->type->typeKind == Base))) {
		type = new Type(Error);
		left = l;
		right = r;
	}

	else if ((l->type->base == Int) && (r->type->base == Float)){
		OpUnary *xf = new OpUnary(r, TO_INT);
		left = l;
		right = xf;
		type = l->type;
	}
	else if ((l->type->base == Float) && (r->type->base == Int)){
		OpUnary *xf = new OpUnary(r, TO_FLOAT);
		left = l;
		right = xf;
		type = l->type;
	}
	else{
		left = l;
		right = r;
		type = new Type(Error);
	}
}

void Assign::print(){
	cout << "(Assign ";
	left->print();
	cout << " ";
	right->print();
	cout << ")";
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

Member::Member() {}
Member::Member(ExpAst* l, Identifier* r){
	left = l;
	right = r;

}

void Member::print(){
	cout << "(Member ";
	left->print();
	cout << " ";
	right->print();
	cout << ")";
}


Arrow::Arrow() {}
Arrow::Arrow(ExpAst* l, Identifier* r){
	left = l;
	right = r;
}

void Arrow::print(){
	cout << "(Arrow ";
	left->print();
	cout << " ";
	right->print();
	cout << ")";
}




Identifier::Identifier(){}
Identifier::Identifier(string x){
	child = x;
}
void Identifier::print(){
	cout<<"(Id \"" << child << "\")";
}


//Array

ArrayRef::ArrayRef() {}
ArrayRef::ArrayRef(ExpAst* l, ExpAst* r){
	left = l;
	right = r;
}

void ArrayRef::print(){
	cout << "(Array ";
	left->print();
	cout << " ";
	right->print();
	cout << ")";
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


