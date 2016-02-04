#include <iostream>
#include <vector>

enum opNameB{OR=1,
			 AND=2,
			 EQ_OP=3,
			 NE_OP=4,
			 LT=5,
			 LE_OP=6,
			 GE_OP=7,
			 PLUS=8,
			 MINUS=9,
			 MULT=10,
			 ASSIGN=11
};


enum opNameU{UMINUS=12,
			 NOT=13,
			 PP=14
};


class abstract_astnode{
public:
  virtual void print () = 0;
  //virtual std::string generate_code(const symbolTable&) = 0;
  //virtual basic_types getType() = 0;
  //virtual bool checkTypeofAST() = 0;
protected:
  // virtual void setType(basic_types) = 0;
private:
  //typeExp astnode_type;
};


////////////////////////////////////////////////////////

class StmtAst: public abstract_astnode{
public:
	void print() = 0;
};

class ExpAst: public abstract_astnode{
public:
	void print() = 0;
};

class RefAst: public abstract_astnode{
public:
	void print() = 0;
};



////////////////////////////////////////////////////////


class BlockStatement: public StmtAst{
public:
	std::vector<StmtAst*> children;
public:
  	void print();
  	BlockStatement();
  	BlockStatement(StmtAst*);
};


class Empty:public StmtAst{
public:
	void print();
};

class Seq: public StmtAst{
	StmtAst*child;
public:
	void print();
	Seq();
	Seq(StmtAst*);
};

class Ass: public StmtAst{
	ExpAst*left,*right;
public:
	void print();
	bool empty;
	Ass();
	Ass(ExpAst*left, ExpAst*right);

};

class Return: public StmtAst{
	ExpAst*child;
public:
	void print();
	Return();
	Return(ExpAst*);
};

class If: public StmtAst{
	ExpAst*first;
	StmtAst*second,*third;
public:
  	void print();
  	If();
  	If(ExpAst*, StmtAst*, StmtAst*);
};


class While: public StmtAst{
  	ExpAst*left;
  	StmtAst*right;
public:
  	void print();
 	While();
  	While(ExpAst*, StmtAst*);
};


class For: public StmtAst{
  	ExpAst*first,*second,*third;
  	StmtAst*child;
public:
	void print();
  	For();
  	For(ExpAst*, ExpAst*, ExpAst*, StmtAst*);
};


////////////////////////////////////////////////////////

class OpBinary: public ExpAst{
	ExpAst*left,*right;
  	opNameB opName;
public:
  	void print();
  	OpBinary();
  	void setArguments(ExpAst*, ExpAst*);
  	OpBinary(ExpAst*, ExpAst*, opNameB);
  	OpBinary(opNameB);
};

class OpUnary: public ExpAst{
	ExpAst* child;
  	opNameU opName;
public:
  	void print();
  	OpUnary();
  	OpUnary(ExpAst*, OpUnary*);
  	OpUnary(ExpAst*, opNameU);
  	OpUnary(opNameU);
};

class Funcall: public ExpAst{
public:
	std::vector<ExpAst*> children;
  	void print();
  	Funcall();
  	Funcall(std::vector<ExpAst*>);
  	Funcall(ExpAst*);
};

class FloatConst: public ExpAst{
	float child;
public:
  	void print();
  	FloatConst();
  	FloatConst(float child);
};

class IntConst: public ExpAst{
	int child;
public:
  	void print();
  	IntConst();
  	IntConst(int child);
};

class StringConst: public ExpAst{
	std::string child;
public:
  	void print();
  	StringConst();
  	StringConst(std::string child);
};


////////////////////////////////////////////////////////

class Identifier: public RefAst{
	std::string  child;
public:
  	void print();
  	Identifier();
  	Identifier(std::string child);
};

class ArrayRef: public RefAst{
	Identifier*left;
  	ExpAst*right;
public:
  	void print();
  	ArrayRef();
  	ArrayRef(Identifier*left, ExpAst*right);
};

class Pointer: public RefAst{
	RefAst*child;
public:
	void print();
	Pointer();
	Pointer(RefAst*);
};

class DeRef: public RefAst{
	RefAst*child;
public:
	void print();
	DeRef();
	DeRef(RefAst*);
};