#include <iostream>

enum opNameB{OR=1,
			 AND=2,
			 EQ_OP=3,
			 NE_OP=4,
			 LT=5,
       		GT = 6,
			 LE_OP=7,
			 GE_OP=8,
			 PLUS=9,
			 MINUS=10,
			 MULT=11,
			 ASSIGN=12,
       PTR_OP = 13,
       OBJ_OP = 17
};


enum opNameU{UMINUS=14,
			 NOT=15,
			 PP=16,
			 DEREF = 18,
			 POINTER = 19
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
class RefAst: public ExpAst{
public:
  void print() = 0;
};



////////////////////////////////////////////////////////


class BlockStmt: public StmtAst{
public:
	std::vector<StmtAst*> children;
public:
  	void print();
  	BlockStmt();
  	BlockStmt(StmtAst*);
};


class Empty:public StmtAst{
public:
	void print();
};

class Seq: public StmtAst{
	std::vector<StmtAst*>children;
public:
	void print();
  void insert_Seq(StmtAst*);
	Seq();
	Seq(StmtAst*);
	Seq(std::vector<StmtAst*>);
};

class Ass: public StmtAst{
	ExpAst *child;
public:
	void print();
	bool empty;
	Ass();
	Ass(ExpAst*child);

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

class Assign: public ExpAst{
  	ExpAst*left;
  	ExpAst*right;
public:
  	void print();
 	Assign();
  	Assign(ExpAst*, ExpAst*);
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
class Pointer: public RefAst{
	RefAst*child;
public:
	void print();
	Pointer();
	Pointer(RefAst*);
};


////////////////////////////////////////////////////////

class Identifier: public RefAst{
	std::string  child;
public:
  	void print();
  	Identifier();
  	Identifier(std::string child);
};


class Member: public RefAst{
  	ExpAst*left;
  	Identifier*right;
public:
  	void print();
 	Member();
  	Member(ExpAst*, Identifier*);
};
class Arrow: public RefAst{
  	ExpAst*left;
  	Identifier*right;
public:
  	void print();
 	Arrow();
  	Arrow(ExpAst*, Identifier*);
};



class ArrayRef: public RefAst{
	ExpAst*left;
  	ExpAst* right;
public:
  	void print();
  	ArrayRef();
  	ArrayRef(ExpAst* left, ExpAst* right);
};


class DeRef: public RefAst{
	RefAst*child;
public:
	void print();
	DeRef();
	DeRef(RefAst*);
};