#include <iostream>
#include <map>
#include <string>
using namespace std;

class Type{
public:
};

class SymbolTableEntry{
public:
	string name;
	int size;
	//Type *rettype;
};


class SymbolTable{
public:
	map <string, SymbolTableEntry*> parameter;
	map <string, SymbolTableEntry*> localvar;
	string entryName;
	Type *rettype;
	void Print();
};

class GlobalTable{
public:
	map <string, SymbolTable> symtables;
	void insertTable(SymbolTable*);
}