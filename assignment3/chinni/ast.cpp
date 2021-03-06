#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <map>
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

map <string, string> str_labels;
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
int x=0;
void BlockStmt::code(int mode){
	// map <string, SymbolTable*>::iterator sit = gtable->funsymtable.begin(); 
	// cout<< gtable->funsymtable.size()<<endl;
	//cout<< sit->second->entryName <<":" <<endl;

	//cout<<x<<endl;
	// advance(sit, x);
	// cout<<sit->second->entryName<<":"<<endl;
	
	for (int i = 0; i < children.size(); i++) {
		children.at(i)->code(0);
	}
	// x++;
}
void Empty::print(){
	// cout << "(Empty)";
}
void Empty::code(int mode){}

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
void Seq::code(int mode){
	for (int i = 0; i < children.size(); i++) {
		children.at(i)->code(0);
	}
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
void Ass::code(int mode){
	if(empty){
		// cout << "(Empty)";
		return;
	}
	// cout << "(Ass ";
	child-> code(0);
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
void Return::code(int mode){
	child->code(0);
}
//If

If::If(){}
If::If(ExpAst* f, StmtAst* s, StmtAst* t){
	first = f;
	second = s;
	third = t;
}
If::If(ExpAst* f, string s)
{
	first = f;
	str = s;
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
vector<int> if_count;
int ifC = 0;
void If::code(int mode){
	if_count.push_back(ifC);
	string tmp = "IF_LOOP" + to_string(if_count[if_count.size() - 1]);
	If* cond_check = new If(first, tmp);
	cond_check->lpcode();
	ifC++;
	third->code(0);
	cout << "j IF_SKIP" << if_count[if_count.size() - 1] << endl<<endl;
	cout << "IF_LOOP" << if_count[if_count.size() - 1] << ": " << endl;
	second->code(0);
	cout << "IF_SKIP" << if_count[if_count.size() - 1] << ": " << endl;
	if_count.pop_back();
}
void If::lpcode()
{             
	first->code(0);
	cout << "lw $t1, 0($sp)" << endl
		 << "bne $t1, $0, " << str << endl << endl;
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
vector<int> while_count;
int whileC=0;
void While::code(int mode){
	while_count.push_back(whileC);
	string tmp = "WHILE_LOOP" + to_string(while_count[while_count.size() - 1]);
	If* cond_check = new If(left, tmp);
	cond_check->lpcode();
	cout << "j WHILE_SKIP" << while_count[while_count.size() - 1] << endl << endl;
	cout << "WHILE_LOOP" << while_count[while_count.size() - 1] << ": " << endl;
	whileC++;
	right->code(0);
	cond_check->lpcode();
	cout << "WHILE_SKIP" << while_count[while_count.size() - 1] << ": " << endl;
	while_count.pop_back();
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
vector <int> for_count;
int forC = 0;
void For::code(int mode){
	for_count.push_back(forC);
	string tmp = "FOR_LOOP" + to_string(for_count[for_count.size() - 1]);
	If* loop_check = new If(second, tmp);
	first->code(0);
	loop_check->lpcode();
	cout << "j FOR_SKIP" << for_count[for_count.size() -  1] << endl <<endl;
	cout << endl << "FOR_LOOP" << for_count[for_count.size() - 1] << ": "<< endl;
	forC++;
	child->code(0);
	third->code(0);
	loop_check->lpcode();
	cout << "FOR_SKIP" << for_count[for_count.size() - 1] << ": "<< endl << endl;
	for_count.pop_back();
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
void OpBinary::code(int mode){
	left->code(0);
  	right->code(0);
  	// cout << endl << " blah "<<opName<<endl;

	cout << "lw $t2, 0($sp)" << endl <<
			"lw $t1, 4($sp)" << endl;
	if(opName == 1)cout << "or $t1, $t1, $t2" << endl;
	if(opName == 2)cout << "and $t1, $t1, $t2" << endl;
	if(opName == 4)cout << "seq $t1, $t1, $t2" << endl;
	if(opName == 7)cout << "sne $t1, $t1, $t2" << endl;
	if(opName == 10)cout << "slt $t1, $t1, $t2" << endl;
	if(opName == 13)cout << "sgt $t1, $t1, $t2" << endl;
	if(opName == 16)cout << "sle $t1, $t1, $t2" << endl;
	if(opName == 19)cout << "sge $t1, $t1, $t2" << endl;		
	if(opName == 22)cout << "add $t1, $t1, $t2" << endl;
	if(opName == 25)cout << "sub $t1, $t1, $t2" << endl;
	if(opName == 28)cout << "mul $t1, $t1, $t2" << endl;
	if(opName == 31)cout << "div $t1, $t1, $t2" << endl;

	//Float binary op
	// if(opName == 5)cout << "seq $f1, $f1, $f2" << endl;
	// if(opName == 8)cout << "sne $f1, $f1, $f2" << endl;
	// if(opName == 11)cout << "slt $f1, &f1, $f2" << endl;
	// if(opName == 14)cout << "sgt $f1, $f1, $f2" << endl;
	// if(opName == 17)cout << "sle $f1, $f1, $f2" << endl;
	// if(opName == 20)cout << "sge $f1, $f1, $f2" << endl;		
	// if(opName == 23)cout << "add $f1, $f1, $f2" << endl;
	// if(opName == 26)cout << "sub $f1, $f1, $f2" << endl;
	// if(opName == 29)cout << "mult $f1, $f1, $f2" << endl;
	// if(opName == 32)cout << "div $f1, $f1, $f2" << endl;
	cout << "addi $sp, $sp, 8" << endl <<
			"addi $sp, $sp, -4" << endl <<
			"sw $t1, 0($sp)" << endl;

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
	type = x->type->copy();
	opName = op;
	if(op == PP){
		type->check = 3;
	}
}
OpUnary::OpUnary(ExpAst * x, OpUnary* y) {
	opName = y->opName;
	if((y->opName == UMINUS) || (y->opName == NOT) || (y->opName == PP)){
		child = x;
		type = x->type->copy();
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
		if(x->type->typeKind != Pointer)type->typeKind = Pointer;
		return;
	}

}
void OpUnary::print(){
	cout<<"("<< get_enum[opName] << " ";
  	child->print();
  	cout<<")";
}
void OpUnary::code(int mode){
	child->code(0);

	cout << "lw $t1, 0($sp)" << endl;
	if(opName == 35)
	{
		cout << "li $t0, 1" << endl <<
				"xor $t1, $t1, $t0" << endl;
	}
	if(opName == 36)
	{
		cout << "addi $t1, $t1, 1" << endl
			 << "addi $t2, $fp, " << child->addr << endl
			 << "sw $t1, 0($t2)" << endl;
	}
	if(opName == 37)
	{	
		cout << "addi $t1, $fp, " << child->addr << endl;
	}
	if(opName == 34)
	{
		cout << "li $t2, -1" << endl;
		cout << "mul $t1, $t1, $t2" << endl;
	}
	if(opName == 38)
	{
		cout << "lw $t1, 0($sp)" << endl
		 << "lw $t1, 0($t1)" << endl;
	}
	cout << "addi $sp, $sp, 4" << endl <<
			"addi $sp, $sp, -4" << endl <<
			"sw $t1, 0($sp)" << endl;
}
//Assign
Assign::Assign() {}
Assign::Assign(ExpAst* l, ExpAst* r){
	if((l->type)->equalassign(r->type)){
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
void Assign::code(int mode){
	// cout << stable->entryName << endl;
	// cout << left->is_right << endl << right->is_right << endl;

	right->code(0);
	left->code(1);
	// if(left->type->isArray == 1){
	// 	cout << "lw $t1, 4($sp)" << endl <<
	// 			"sw $t1, 0($t9)" << endl <<
	// 			"addi $sp, $sp, 8" << endl <<
	// 			"addi $sp, $sp, -4" << endl <<
	// 			"sw $t1, 0($sp)" << endl<<endl;
	// 	addr = left->addr;
	// 	return;
	// }
	cout << "lw $t1, 4($sp)" << endl <<
			"lw $t2, 0($sp)" << endl <<
			"sw $t1, 0($t2)" << endl <<
			"addi $sp, $sp, 8" << endl <<
			"addi $sp, $sp, -4" << endl;
			if(mode == 0) cout << "sw $t1, 0($sp)" << endl<<endl;
			else cout << "sw $t2, 0($sp)" << endl<<endl;
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

void type_change(Type* &a, Type* &b)
{
	a->base = b->base;
}

void Funcall::code(int mode)
{	
	string funName = ((Identifier*)(children[0]))->child;
	if(funName == "printf"){
		for(int i = 1; i < children.size(); i++)
			{
				children[i]->code(0);
				// Type* required = pit->second->idType;
				// Type* present = children[i]->type;
				// type_change(present, required);
				// pit++;
				if(children[i]->type->base == 0)
				{
					cout << "lw $t1, 0($sp)" << endl
						 // << "lw $t1, 0($t1)" << endl
						 << "move $a0, $t1" << endl
						 << "li $v0, 1" << endl
						 <<"syscall" << endl << endl
						 << "addi $sp, $sp, 4" << endl;	
				}
				if(children[i]->type->base == 2)
				{
					cout << "lw $t1, 0($sp)" << endl
						 // << "lw $t1, 0($t1)" << endl
						 << "move $a0, $t1" << endl
						 << "li $v0, 4" << endl
						 <<"syscall" << endl << endl
						 << "addi $sp, $sp, 4" << endl;	
				}
				
			}
		return;
	}
	SymbolTable* tmp = gtable->funsymtable.find(funName)->second;
	map <string, SymbolTableEntry*>::iterator pit = tmp->parameters.begin();


	cout<<"addi $sp, $sp, -" << tmp->size() <<endl; //save ret.val
		
	for(int i = children.size() - 1; i >= 1; i--)
	{
		children[i]->code(0);
		// Type* required = pit->second->idType;
		// Type* present = children[i]->type;
		// type_change(present, required);
		// pit++;
	}
	// cout<<"sw $fp, 0($sp)"<<endl;
	cout<<"jal "<<funName<<endl;
}

///////////////////////////////////////////////////////////////////////

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
void Pointer::code(int mode){
	child->code(0);
	cout << "lw $t1, 0($sp)" << endl
		 << "lw $t1, 0($t1)" << endl
		 << "addi $sp, $sp, 4" << endl
		 << "addi $sp, $sp, -4" << endl
		 << "sw $t1, 0($sp)" << endl;
}
//Float Constants

FloatConst::FloatConst(float x){
	child = x;
}
void FloatConst::print(){
	cout << "(FloatConst "<< child << ")";
}
void FloatConst::code(int mode)
{
	cout<<"li $f0, "<< child <<endl;
	cout<<"addi $sp, $sp, -4"<<endl;
	cout<<"s.s $f0, 0($sp)"<<endl;
}
//Int Constants

IntConst::IntConst(){}
IntConst::IntConst(int x){
	child = x;
}
void IntConst::code(int mode){
	cout << "addi $sp, $sp, -4" << endl <<
			"addi $t1, $0, "<< child << endl <<
			"sw $t1, 0($sp)" << endl;
	return;
}
void IntConst::print(){
	cout << "(IntConst " << child << ")";
}

//String Constants


int strn_num = 0;
StringConst::StringConst(string x){
	child = x;
	str_labels[x] = ("ss" + to_string(strn_num));
	strn_num++;
}
void StringConst::print(){
	cout << "(StringConst " << child << ")";
}
void StringConst::code(int mode){
	map<string, string>::iterator it;
	it = str_labels.find(child);
	string s = it->second;
	cout << "la $t1, " << s << endl 
		 << "addi $sp, $sp, -4" << endl
		 << "sw $t1, 0($sp)" << endl;
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
void Member::code(int mode){
	left->code(1);
	cout << " ";
	string strName = left->type->structType;
	SymbolTable* tmp = gtable->strsymtable.find(strName)->second;
	map <string, SymbolTableEntry*>::iterator pit = tmp->localvars.find(right->child);
	int off = pit->second->offset;
	if(mode == 0)
	{
		cout << "lw $t1, 0($sp)" << endl
			 << "addi $t1, $t1, " << off << endl
			 << "lw $t1, 0($t1)" << endl
			 << "addi $sp, $sp, 4" << endl
			 << "addi $sp, $sp, -4" << endl
			 << "sw $t1, 0($sp)" << endl;
	}
	if(mode == 1)
	{
		cout << "lw $t1, 0($sp)" << endl
			 << "addi $t1, $t1, " << off << endl
			 << "addi $sp, $sp, 4" << endl
			 << "addi $sp, $sp, -4" << endl
			 << "sw $t1, 0($sp)" << endl;
	}
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
void Arrow::code(int mode){
	left->code(0);
	cout << " ";
	right->code(1);
}



Identifier::Identifier(){}
Identifier::Identifier(string x){
	child = x;
}

void Identifier::code(int mode){
	// cout << child << "  " << this->is_left << endl;
	map <string, SymbolTableEntry*>::iterator pit = stable->parameters.find(child);
	if(pit != stable->parameters.end()){
		// int tmp = 4 + pit->second->offset;
		// if(this->is_left == 0){
		// 	cout << "addi $sp, $sp, -4" << endl <<
		// 		"addi $t1, $fp, "<< tmp << endl <<
		// 		"lw $t1, 0($t1)" << endl <<
		// 		"sw $t1, 0($sp)" << endl<<endl;
		// }
		// else{
		// 	cout << "addi $sp, $sp, -4" << endl <<
		// 			"addi $t1, $fp, "<< tmp << endl <<
		// 			"sw $t1, 0($sp)" << endl<<endl;
		// 	}
		// return;
		int tmp = 8 + pit->second->offset;
		if(mode ==  1){
			cout << "addi $sp, $sp, -4" << endl <<
				"addi $t1, $fp, "<< tmp << endl <<
				"sw $t1, 0($sp)" << endl<<endl;
				return;
		}

		cout << "addi $sp, $sp, -4" << endl <<
				"addi $t1, $fp, "<< tmp << endl <<
				"lw $t1, 0($t1)" << endl <<
				"sw $t1, 0($sp)" << endl<<endl;

		return;



	}
	pit = stable->localvars.find(child);
	if(pit != stable->localvars.end()){
		// int tmp = pit->second->offset;
		// if(this->is_left == 0){
		// 	cout << "addi $sp, $sp, -4" << endl <<
		// 			"addi $t1, $fp, " << tmp << endl <<
		// 			"lw $t1, 0($t1)" << endl <<
		// 			"sw $t1, 0($sp)" << endl<<endl;
		// }
		// else{
		// 	cout << "addi $sp, $sp, -4" << endl <<
		// 			"addi $t1, $fp, " << tmp << endl <<
		// 			"sw $t1, 0($sp)" << endl<<endl;
		// }
		// return;\

		int tmp = pit->second->offset;
		if(mode ==  1){
			cout << "addi $sp, $sp, -4" << endl <<
				"addi $t1, $fp, "<< tmp << endl <<
				"sw $t1, 0($sp)" << endl<<endl;
				return;
		}
		cout << "addi $sp, $sp, -4" << endl <<
				"addi $t1, $fp, "<< tmp << endl <<
				"lw $t1, 0($t1)" << endl <<
				"sw $t1, 0($sp)" << endl<<endl;

		addr = tmp;
		return;

// =======
// 		int tmp = 4 + pit->second->offset;
// 		if(this->is_left == 0){
// 			cout << "addi $sp, $sp, -4" << endl <<
// 				"addi $t1, $fp, "<< tmp << endl <<
// 				"lw $t1, 0($t1)" << endl <<
// 				"sw $t1, 0($sp)" << endl<<endl;
// 		}
// 		else{
// 			cout << "addi $sp, $sp, -4" << endl <<
// 					"addi $t1, $fp, "<< tmp << endl <<
// 					"sw $t1, 0($sp)" << endl<<endl;
// 			}
// 		return;
// 	}
// 	pit = stable->localvars.find(child);
// 	if(pit != stable->localvars.end()){
// 		int tmp = pit->second->offset;
// 		if(this->is_left == 0){
// 			cout << "addi $sp, $sp, -4" << endl <<
// 					"addi $t1, $fp, " << tmp << endl <<
// 					"lw $t1, 0($t1)" << endl <<
// 					"sw $t1, 0($sp)" << endl<<endl;
// 		}
// 		else{
// 			cout << "addi $sp, $sp, -4" << endl <<
// 					"addi $t1, $fp, " << tmp << endl <<
// 					"sw $t1, 0($sp)" << endl<<endl;
// 		}
// 		return;
// >>>>>>> c084be6942353b23eeb61db5f019e3f172175459
	}
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
void ArrayRef::code(int mode){
	left->code(1);
	right->code(0);

	// cout << left->type->num_type_pointers << "   " << left->type->isArray << "  "<< left->type->arrayVector.size() << endl;

	int tmp = left->type->size();
	for(int i = 1; i < left->type->arrayVector.size(); i++){
		tmp = tmp*left->type->arrayVector[i];
	}

	cout << "lw $t1, 0($sp)" << endl <<
			"lw $t2, 4($sp)" << endl <<
			"li $t3, "<< tmp<< endl << 
			"mul $t1, $t1, $t3"<< endl <<
			"add $t2, $t2, $t1" <<  endl <<
			"addi $sp, $sp, 8" << endl;

	if(mode == 0){
		cout << "lw $t2, 0($t2)" << endl;
		}
			cout << "addi $sp, $sp, -4" << endl <<
				"lw $t2, 0($sp)" << endl;

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

void DeRef::code(int mode){
	child->code(0);
}


void gen_func(SymbolTable* st){
	if(st->entryName == "main")
	{
		cout << "		.text"<<endl;
	}
	cout << st->entryName << ":" << endl;
	cout << "addi $sp, $sp, -4" << endl
		 << "sw $ra, 0($sp)" << endl
		 << "addi $sp, $sp, -4" << endl
		 << "sw $fp, 0($sp)" << endl
		 << "move $fp, $sp" <<endl;

	map <string, SymbolTableEntry*>::iterator sit;
	int tmp = 0;
	for(sit = st->localvars.begin(); sit != st->localvars.end(); sit++){
		if(tmp >= sit->second->offset)tmp = sit->second->offset;
	}
	cout << "addi $sp, $sp, "<< tmp << endl;
	cout << endl;
}


void gen_funend(SymbolTable* st)
{
	if(st->entryName != "main")
	{
		cout << "lw $ra, 4($fp)" << endl
		 << "jr $ra" << endl << endl;	
	}
	else
	{
		cout << "li $v0, 10" << endl
			 << "syscall" << endl;
	}
		
}

void strn_data()
{
	cout << "		.data"<<endl;
	map<string, string>::iterator it;
	for(it = str_labels.begin(); it!=str_labels.end(); ++it)
	{
		cout << it->second << ":  .asciiz  " << it->first << endl;	
	}
}
