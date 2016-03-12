#include "st.h"
#include <iostream>
using namespace std;

//type
Type::Type(){}

Type::Type(Kind tag, Basetype basetype){
	this->typeKind = tag;
	this->base = basetype;
}
Type::Type(Kind tag, Basetype basetype, string structtype){
	this->typeKind = tag;
	this->base = basetype;
	this->structType = structtype;
}
int Type::size() {
	if (tag == Base)	{
		if (basetype == Void)
			return 0;
		return 4;
	}
}

//symbol table entry
SymbolTableEntry::SymbolTableEntry(){}

//Symbol table for function and struct
SymbolTable::SymbolTable(){};







map<string, SymbolTableEntry*> paraMap;
SymbolTable* stable;
SymbolTableEntry* ptable;
int offset = 0;
Type* retType;
GlobalTable *gtable = new GlobalTable()