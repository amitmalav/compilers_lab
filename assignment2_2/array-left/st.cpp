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
int isConstant = 0;
int paddr= 0;
int lvalue = 0;

string gettype[] = {
	"int",
	"float",
	"void",
	"struct"
};


//type
Type::Type(){}
Type::Type(Kind tag){
	this->typeKind = tag;
	num_type_pointers = 0;
	check = 0;
	isArray = 0;
}
Type::Type(Kind tag, Basetype basetype){
	this->typeKind = tag;
	this->base = basetype;
	num_type_pointers = 0;
	check = 0;
	isArray = 0;
}
Type::Type(Kind tag, Basetype basetype, string structtype){
	this->typeKind = tag;
	this->base = basetype;
	this->structType = structtype;
	num_type_pointers = 0;
	check = 0;
	isArray = 0;
}
Type* Type::copy(){
	Type * copied = new Type(typeKind, base);
	copied->check = check;
	copied->isArray = isArray;
	if (typeKind == Pointer){
		if(base == Struct){
			copied->structType = structType;
		}
		copied->num_type_pointers = num_type_pointers;
	}
	if(typeKind == Base){
		if(base == Struct){
			copied->structType = structType;
		}
	}
	return copied;
}
bool Type::equal(Type* t){
	int assign = 0;
	if(typeKind != t->typeKind)return false;
	if(typeKind == Base){
		// if(!((this->getType() != "int" || this->getType() != "float")&& (t->getType() != "int" || t->getType() != "float"))){
		// 	return true;
		// }
		// else 
			return(this->getType() == t->getType());
	}
	if(typeKind == Pointer){
		//cout << "blah0"<<endl;
		if(assign == 1)if(isArray == 1 && t->isArray == 1)return false;
		if(this->getType() != t->getType()){

			if((this->getType() == "void")&&(t->getType() != "void")){
				return (this->num_type_pointers == 1);
			}
			if((this->getType() != "void")&&(t->getType() == "void")){
				return (t->num_type_pointers == 1);
			}
			else return false;
		}
		else{
			return (this->num_type_pointers == t->num_type_pointers);
		}
	}
	return true;
}
bool Type::equalassign(Type* t){
	int assign = 1;
	if(typeKind != t->typeKind)return false;
	if(typeKind == Base){
		// if(!((this->getType() != "int" || this->getType() != "float")&& (t->getType() != "int" || t->getType() != "float"))){
		// 	return true;
		// }
		// else 
			return(this->getType() == t->getType());
	}
	if(typeKind == Pointer){
		//cout << "blah0"<<endl;
		if(assign == 1)if(isArray == 1 && t->isArray == 1)return false;
		if(this->getType() != t->getType()){

			if((this->getType() == "void")&&(t->getType() != "void")){
				return (this->num_type_pointers == 1);
			}
			if((this->getType() != "void")&&(t->getType() == "void")){
				return (t->num_type_pointers == 1);
			}
			else return false;
		}
		else{
			return (this->num_type_pointers == t->num_type_pointers);
		}
	}
	return true;
}



int Type::size(){
	if(typeKind == Pointer && isArray == 0){
		return 4;
	}
	if (typeKind == Base  || ((typeKind == Pointer) &&(isArray = 1))){
		if (base == Void)
			return 0;
		if(base == Struct){
			map <string, SymbolTable*>::iterator it = gtable->strsymtable.find(structType);
			if(it != gtable->strsymtable.end()){
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
			//cout << "blah" <<structType << "bl"<<endl;
			return structType;
		}
		return gettype[base];
	}
}

//symbol table entry
SymbolTableEntry::SymbolTableEntry(){}
SymbolTableEntry::SymbolTableEntry(int addr){
	this->addr = addr;
}
int SymbolTableEntry::size(){
	int ssize = 1;
	if(isArray == 1){
		for(std::vector<int>::iterator it = arrayVector.begin(); it != arrayVector.end(); ++it){
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
			 	int t;
				if(isArray == 1){
					t = numPointers - arrayVector.size();
				}
			 	for(int i = 0; i < t; i++){
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
				map <string, SymbolTable*>::iterator it1 = gtable->strsymtable.find(it->second->idType->getType());
				if(it1 != gtable->strsymtable.end()){
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

bool SymbolTable::checkScope(string var){
	if (parameters.find(var)!=parameters.end() || localvars.find(var)!=localvars.end())
		return true;
	else return false;
}

Type* SymbolTable::getType(string var){
	if (parameters.find(var) != parameters.end())
		return parameters[var]->idType;
	else if(localvars.find(var) != localvars.end()){
		return localvars[var]->idType;	
	}
}

Type * SymbolTable::getParaByInd(int i) {
	for (map<string, SymbolTableEntry*>::iterator it = parameters.begin(); it != parameters.end(); it++) {
		if ((it->second)->addr == i) {
			return (it->second)->idType;
		}
	}
}


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
	for(map <string, SymbolTable*>::iterator it = strsymtable.begin(); it != strsymtable.end(); ++it){
			it->second->print();
			cout << endl<<endl;
	}
	for(map <string, SymbolTable*>::iterator it = funsymtable.begin(); it != funsymtable.end(); ++it){
			it->second->print();
			cout << endl<<endl;
	}


}
void GlobalTable::insertTablefun(SymbolTable* st){
	funsymtable[st->entryName] = st;
	symtable[st->entryName] = st;
}
void GlobalTable::insertTablestr(SymbolTable* st){
	strsymtable[st->entryName] = st;
	symtable[st->entryName] = st;
}



