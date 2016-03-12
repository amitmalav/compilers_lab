#include <iostream>
#include <map>
#include <string>
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
	Type(Kind, Basetype);
	Type(Kind,Basetype, string);
	void Print();
	bool equal(Type*);
	Type* copy();
	int size();
	bool isNumeric();

};

class SymbolTableEntry{
public:
	string name;
	int entrysize;
	int index;
	int addr;
	Type* idType;
	int numPointers;
	SymbolTableEntry();
	int size();
	SymbolTableEntry(int addr, Type* idType);
	SymbolTableEntry(int addr, Type* idType, string name);
	void Print();
};


class SymbolTable{
public:
	map <string, SymbolTableEntry*> parameters;
	map <string, SymbolTableEntry*> localvars;
	string entryName;
	Type *retType;
	int numPointers;
	int isStruct;
	void Print();
	SymbolTable();
};

class GlobalTable{
public:
	map <string, SymbolTable> symtables;
	void insertTable(SymbolTable*);
};