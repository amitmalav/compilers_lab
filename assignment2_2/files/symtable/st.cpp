#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;


map<string, SymbolTableEntry*> paraMap;
SymbolTable* stable;
SymbolTableEntry* ptable;
int offset = 0;
Type* retType;
GlobalTable *gtable = new GlobalTable();
int pusharraysize = 0;

string gettype[] = {
	"int",
	"float",
	"void",
	"struct"
};


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
int Type::size(){
	if(typeKind == Pointer){
		return 4;
	}
	if (typeKind == Base ){
		if (base == Void)
			return 0;
		if(base == Struct){
			map <string, SymbolTable*>::iterator it = gtable->symtable.find(structType);
			if(it != gtable->symtable.end()){
				return it->second->size();
			}
			else return 0;
		}
		return 4;
	}
	return 0;
}
string Type::getType(){
	if (typeKind == Base|| typeKind == Pointer){
		if(base == Struct){
			return structType;
		}
		return gettype[base];
	}
}

//symbol table entry
SymbolTableEntry::SymbolTableEntry(){}
int SymbolTableEntry::size(){
	int ssize = 1;
	if(isArray == 1){
		for(std::vector<int>::iterator it = arrayVector.begin(); it != arrayVector.end(); it++){
			 ssize *= *it;
		}
		ssize *= idType->size();
		return ssize;
	}
	return idType->size();
}


void SymbolTableEntry::print(){
	//sname, type, size, offset
	int ssize = idType->size();
	string s = "";
	if(isArray == 1){
		for(std::vector<int>::iterator it = arrayVector.begin(); it != arrayVector.end(); it++){
			stringstream ss;
			 if(it == arrayVector.begin()){
			 	string tmp = idType->getType();
			 	for(int i = 0; i < numPointers; i++){
			 		tmp = tmp + "*";
			 	}
			 	ss << *it;
			 	s = "array(" + tmp + ", "+ ss.str() + ")";
			 	ss.str("");
			 }
			 else{
			 	ss << *it;
			 	s = "array(" + s + ", "+ ss.str() + ")";
			 	ss.str("");
			 }
		}
	}
	else{
		s = idType->getType();
		for(int i = 0; i < numPointers; i++){
			s = s + "*";
		}

	}
	cout << name << "		"<< s << "		"<<this->size() << "		"<<offset;
	
}




//Symbol table for function and struct
SymbolTable::SymbolTable(){};
int SymbolTable::size(){
	int ssize = 0;
	if(isStruct == 1){
		
		for(map<string, SymbolTableEntry*>::iterator it = localvars.begin(); it != localvars.end(); ++it){
			if(it->second->idType->base == Struct && (it->second->numPointers == 0)){
				map <string, SymbolTable*>::iterator it1 = gtable->symtable.find(it->second->idType->getType());
				if(it1 != gtable->symtable.end()){
					if(it1->second->isStruct == 1 && (it->second->idType->getType() == entryName))
						return 0;
				}
			}


			ssize = ssize + it->second->size();
				    // iterator->first = key
			    // iterator->second = value
			    // Repeat if you also want to iterate through the second map.
		}
		return ssize;		
	}
	else return -1;
};
void SymbolTable::print(){
	//sname, var/fun, para/local, size
	if(isStruct == 1){

		cout << "Struct: "<<entryName <<"		" <<"struct" << "		"<< this->size()<<endl;
		for(map <string, SymbolTableEntry*>::iterator it = localvars.begin(); it != localvars.end(); ++it){
			cout << "Struct member: "; it->second->print(); cout << endl;
		}
	}
	else{
		string tmp = retType->getType();

		for(int i = 0; i < numPointers; i++){
			 		tmp = tmp + "*";
			 	}
		cout << "Function: "<<entryName <<"		" << tmp<< "		"<< this->size()<<endl;
		for(map <string, SymbolTableEntry*>::iterator it = parameters.begin(); it != parameters.end(); ++it){
			cout << "Function para: "; it->second->print(); cout << endl;
		}
		for(map <string, SymbolTableEntry*>::iterator it = localvars.begin(); it != localvars.end(); ++it){
			cout << "Local vars: "; it->second->print(); cout << endl;
		}
	}
}
GlobalTable::GlobalTable(){}
void GlobalTable::print(){
	for(map <string, SymbolTable*>::iterator it = symtable.begin(); it != symtable.end(); ++it){
			it->second->print();
			cout << endl<<endl;
	}


}
void GlobalTable::insertTable(SymbolTable* st){
	symtable[st->entryName] = st;
}


