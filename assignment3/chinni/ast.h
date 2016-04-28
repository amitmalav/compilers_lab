#include <iostream>
#include <string>

enum opNameB{
  OR = 1,
  AND = 2,
  EQ_OP = 3,
  EQ_OP_INT = 4,
  EQ_OP_FLOAT = 5,
  NE_OP = 6,
  NE_OP_INT = 7,
  NE_OP_FLOAT = 8,
  LT = 9,
  LT_INT = 10,
  LT_FLOAT = 11,
  GT = 12,
  GT_INT = 13,
  GT_FLOAT = 14,
  LE_OP = 15,
  LE_OP_INT = 16,
  LE_OP_FLOAT =17,
  GE_OP = 18,
  GE_OP_INT = 19,
  GE_OP_FLOAT = 20,
  PLUS = 21,
  PLUS_INT = 22,
  PLUS_FLOAT = 23,
  MINUS = 24,
  MINUS_INT = 25,
  MINUS_FLOAT = 26,
  MULT = 27,
  MULT_INT = 28,
  MULT_FLOAT = 29,
  DIV = 30,
  DIV_INT = 31,
  DIV_FLOAT = 32,
  PTR_OP = 33
};


enum opNameU{
  UMINUS=34,
			 NOT=35,
			 PP=36,
			 DEREF = 37,
			 POINTER = 38,
       TO_FLOAT = 39,
       TO_INT = 40
};


class abstract_astnode{
public:
  Type* type;
  int addr;
  virtual void print () = 0;
  virtual void code () = 0;
  
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
  void code() = 0;
  
};

class ExpAst: public abstract_astnode{
public:
  int lvalue;
  int is_left = 0;
	void print() = 0;
  void code() = 0;
};
class RefAst: public ExpAst{
public:
  void print() = 0;
  void code() = 0;
  
};



////////////////////////////////////////////////////////


class BlockStmt: public StmtAst{
public:
	std::vector<StmtAst*> children;
  	void print(); void code();
  	BlockStmt();
  	BlockStmt(StmtAst*);
};


class Empty:public StmtAst{
public:
	void print(); void code();
  
};

class Seq: public StmtAst{
public:
	std::vector<StmtAst*>children;
	void print(); void code();

  void insert_Seq(StmtAst*);
	Seq();
	Seq(StmtAst*);
	Seq(std::vector<StmtAst*>);
};

class Ass: public StmtAst{
public:
  ExpAst *child;
	void print(); void code();
	bool empty;
	Ass();
	Ass(ExpAst*child);

};

class Return: public StmtAst{
public:
  ExpAst*child;
	void print(); void code();
	Return();
	Return(ExpAst*, Type*);
};

class If: public StmtAst{
public:
  ExpAst*first;
  StmtAst*second,*third;
  string str;
  	void print(); void code();
  	void lpcode();
  	If();
  	If(ExpAst*, string);
  	If(ExpAst*, StmtAst*, StmtAst*);
};


class While: public StmtAst{
public:
    ExpAst*left;
    StmtAst*right;
  	void print(); void code();
 	While();
  	While(ExpAst*, StmtAst*);
};


class For: public StmtAst{
public:
    ExpAst*first,*second,*third;
    StmtAst*child;
	void print(); void code();
  	For();
  	For(ExpAst*, ExpAst*, ExpAst*, StmtAst*);
};


////////////////////////////////////////////////////////





class OpBinary: public ExpAst{
public:
  ExpAst*left,*right;
    opNameB opName;
  	void print(); void code();
  	OpBinary();
  	OpBinary(ExpAst*, ExpAst*, opNameB);
  	OpBinary(opNameB);
};

class OpUnary: public ExpAst{
public:
  ExpAst* child;
  opNameU opName;
  	void print(); void code();
  	OpUnary();
  	OpUnary(ExpAst*, OpUnary*);
  	OpUnary(ExpAst*, opNameU);
  	OpUnary(opNameU);
};

class Assign: public ExpAst{
public:
    ExpAst*left;
    ExpAst*right;
  	void print(); void code();
  	Assign();
  	Assign(ExpAst*, ExpAst*);
};

class Funcall: public ExpAst{
public:
	std::vector<ExpAst*> children;
  	void print(); void code();
  	Funcall();
  	Funcall(std::vector<ExpAst*>);
  	Funcall(ExpAst*);
};

class FloatConst: public ExpAst{
public:
  float child;
  	void print(); void code();
  	FloatConst();
  	FloatConst(float child);
};

class IntConst: public ExpAst{
public:
  int child;
  	void print(); void code();
  	IntConst();
  	IntConst(int child);
};

class StringConst: public ExpAst{
public:
  std::string child;
  	void print(); void code();
  	StringConst();
  	StringConst(std::string child);
};
class Pointer: public RefAst{
public:
  RefAst*child;
	void print(); void code();
	Pointer();
	Pointer(RefAst*);
};


////////////////////////////////////////////////////////

class Identifier: public RefAst{
public:
    std::string  child;
  	void print(); void code();
  	Identifier();
  	Identifier(std::string child);
};


class Member: public RefAst{
public:
    ExpAst*left;
    Identifier*right;
  	void print(); void code();
 	  Member();
  	Member(ExpAst*, Identifier*);
};
class Arrow: public RefAst{
public:
    ExpAst*left;
    Identifier*right;
  	void print(); void code();
 	Arrow();
  	Arrow(ExpAst*, Identifier*);
};



class ArrayRef: public RefAst{
public:
  ExpAst*left;
    ExpAst* right;
  	void print(); void code();
  	ArrayRef();
  	ArrayRef(ExpAst* left, ExpAst* right);
};


class DeRef: public RefAst{
public:
  RefAst*child;
	void print(); void code();
	DeRef();
	DeRef(RefAst*);
};