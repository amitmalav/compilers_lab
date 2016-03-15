#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

enum Kind {
	Base, Pointer, Error, Ok
};
enum Basetype {
	Int, Float, Void, Struct
};


class Type{
public:
	int sizeType;
	Kind typeKind;
	Basetype base;
	string structType;
	int num_type_pointers;
	int check; //1 for &, 2 for *, 3 for - or !, funcall
	Type();
	Type(Kind);
	Type(Kind, Basetype);
	Type(Kind,Basetype, string);
	Type* copy();
	bool equal(Type*);
	string getType();
	int size();
};

class SymbolTableEntry{
public:
	string name;
	int entrysize;
	int index;
	int addr;
	int offset;
	int isArray;
	std::vector <int> arrayVector;
	Type* idType;
	int numPointers;
	SymbolTableEntry();
	SymbolTableEntry(int addr);
	int size();
	void print();
};


class SymbolTable{
public:
	map <string, SymbolTableEntry*> parameters;
	map <string, SymbolTableEntry*> localvars;
	string entryName;
	Type *retType;
	int numPointers;
	int isStruct;
	int size();
	void print();
	bool checkScope(string);
	Type* getType(string);
	Type * getParaByInd(int);
	SymbolTable();
};

class GlobalTable{
public:
	map <string, SymbolTable*> funsymtable;
	map <string, SymbolTable*> strsymtable;
	map <string, SymbolTable*> symtable;
	void insertTablefun(SymbolTable*);
	void insertTablestr(SymbolTable*);
	GlobalTable();
	void print();
};