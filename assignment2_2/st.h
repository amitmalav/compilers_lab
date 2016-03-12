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
	Type();
	Type(Kind, Basetype);
	Type(Kind,Basetype, string);
	string getType();
	int size();
};

class SymbolTableEntry{
public:
	string name;
	int entrysize;
	int index;
	int addr;
	int isArray;
	std::vector <int> arrayVector;
	Type* idType;
	int numPointers;
	SymbolTableEntry();
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
	SymbolTable();
};

class GlobalTable{
public:
	map <string, SymbolTable*> symtable;
	void insertTable(SymbolTable*);
	GlobalTable();
	void print();
};