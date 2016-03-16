%scanner Scanner.h
%scanner-token-function d_scanner.lex()
%token STRUCT VOID INT FLOAT RETURN INT_CONSTANT FLOAT_CONSTANT STRING_LITERAL OR_OP AND_OP EQ_OP NE_OP LE_OP GE_OP INC_OP PTR_OP IF ELSE WHILE FOR IDENTIFIER OTHER

%polymorphic type1 : Type*; SymbolTableEntry : SymbolTableEntry*; StmtAst : StmtAst*; ExpAst : ExpAst*; RefAst : RefAst*; Int : int; Float : float; String : std::string;

%type <StmtAst> translation_unit function_definition struct_specifier compound_statement statement_list statement assignment_statement selection_statement iteration_statement

%type <ExpAst>  expression logical_and_expression equality_expression relational_expression additive_expression multiplicative_expression unary_expression postfix_expression primary_expression expression_list unary_operator logical_or_expression

//%type <RefAst> l_expression

%type <Int> INT_CONSTANT

%type <Float> FLOAT_CONSTANT

%type <String>	STRING_LITERAL IDENTIFIER  fun_declarator 
%type <type1> type_specifier VOID INT FLOAT
%type <SymbolTableEntry> declarator
%%

translation_unit_new
		: translation_unit
		{
			cout << endl << endl<< endl<< endl;
			gtable->print();
		}
		;



translation_unit
        :  struct_specifier
 		| function_definition
 		| translation_unit function_definition
        | translation_unit struct_specifier
        ;

struct_specifier 
        : STRUCT IDENTIFIER '{'
        {
        	if (gtable->strsymtable.find($2) != gtable->strsymtable.end())
        	{
				cout << "Error:: On line " << d_scanner.lineNr() << " Redefinition of Struct: " << $2  << endl;
				exit(0);
			}
        	stable = new SymbolTable();
        	stable->entryName = $2;
        	stable->isStruct = 1;
        	stable->retType = new Type(Kind::Base, Basetype::Struct, $2);
        	offset = 0;
        } declaration_list '}' ';'
        {
        	stable->localvars = paraMap;
        	gtable->insertTablestr(stable);
        	paraMap.clear();
        }
        ;



function_definition
		: type_specifier
		{
			stable = new SymbolTable();
			stable->retType = $1;
			stable->numPointers = 0;
			stable->isStruct = 0;
			offset = 0;
			paddr = 0;
		} 
		fun_declarator
		{
			map <string, SymbolTable*>::iterator it = gtable->funsymtable.find($3);
			if(it != gtable->funsymtable.end()){
				if(it->second->isStruct == 0){
					cout << "Error:: On line " << d_scanner.lineNr() << " Redefinition of Function: " << $3  << endl;
					exit(0);
				}
			}
			stable->entryName = $3;
			stable->parameters = paraMap;
			paraMap.clear();
			offset = 0;
		}
		compound_statement
		{	
			stable->localvars = paraMap;
			$$ = $5;
        	//stable->print();
        	gtable->insertTablefun(stable);
        	paraMap.clear();
        	$$->print();
        	cout << endl;
		}
		;

type_specifier                   // This is the information that gets associated with each identifier
        : VOID
        {
        	$$ = new Type(Kind::Base, Basetype::Void);
        	retType = 	$$;
        } 
        | INT
        {
        	$$ = new Type(Kind::Base, Basetype::Int);
        	retType = 	$$;
        }   
		| FLOAT
		{	
        	$$ = new Type(Kind::Base, Basetype::Float);
        	retType = 	$$;
        } 
        | STRUCT IDENTIFIER
        {	
        	$$ = new Type(Kind::Base, Basetype::Struct, $2);
        	retType = 	$$;
        }
        ;

fun_declarator
		: IDENTIFIER '(' parameter_list ')'
		{
			$$ = $1;
		} 
		| IDENTIFIER '(' ')'
		{	
			$$ = $1;
		} 
	    | '*'
	    {
	    	stable->numPointers +=1;
	    	stable->retType->num_type_pointers++;
	    	stable->retType->typeKind = Pointer;
	    }
	    fun_declarator{
	    	$$ = $3;
	    }  //The * is associated with the 
		;                      //function name


parameter_list
		: parameter_declaration 
		| parameter_list ',' parameter_declaration 
		;

parameter_declaration
		: type_specifier
		{
			ptable = new SymbolTableEntry(paddr);
			ptable->idType = $1;
			ptable->numPointers = 0;
			ptable->isArray = 0;
		}
		declarator
		{

			if(ptable->idType->base == Void){
				if(ptable->idType->typeKind != Pointer){
					cout << "Error:: On line " << d_scanner.lineNr() << " \'"<< ptable->name <<"\' has incomplete type"  << endl;
					exit(0);
				}
			}
			map <string, SymbolTableEntry*>::iterator pit = paraMap.find(ptable->name);
			if(pit != paraMap.end()){
					string tmp = pit->second->idType->getType();
				 	int t;
					if(pit->second->isArray = 1){
						t = pit->second->numPointers - pit->second->arrayVector.size();
					}
				 	for(int i = 0; i < t; i++){
				 		tmp = tmp + "*";
				 	}
					cout << "Error:: On line " << d_scanner.lineNr() << " '"<< ptable->name <<"\' has a previous declaration as \'"<< tmp << " " <<ptable->name << "\'"<< endl;
					exit(0);

			}
			if(ptable->idType->base == Struct){
				map <string, SymbolTable*>::iterator it = gtable->symtable.find(ptable->idType->structType);
				if(it == gtable->symtable.end()){
					//remove the comments to let undeclared struct pointer pass as argument
						//if(ptable->numPointers == 0){
							cout << "Error:: On line " << d_scanner.lineNr() << " \'"<< ptable->name <<"\' has incomplete type"  << endl;
							exit(0);
						//}
				}
			}

			offset += ptable->size();
			ptable->offset = offset;
			paddr += 1; 
			paraMap[ptable->name] = $3;
		}
	    ;

declarator
		: IDENTIFIER
		{	
			$$ = ptable;
			ptable->name = $1;

		}
		| declarator '[' primary_expression']'
		{
			if(isConstant == 0){
				cout << "Error:: On line " << d_scanner.lineNr() << ": only int constants are allowed inside array expression" << endl;
				exit(0);
			}

			ptable->isArray = 1;
			ptable->numPointers++;
			ptable->idType->num_type_pointers++;
			ptable->idType->typeKind = Pointer;
			ptable->idType->isArray = 1;
			ptable->arrayVector.push_back((int)pusharraysize);
		} // check separately that it is a constant //  yet to be done
        | '*'
        {
	    	ptable->numPointers +=1;
	    	ptable->idType->num_type_pointers++;
	    	ptable->idType->typeKind = Pointer;
	    }
        declarator
        {
        	$$ = $3;
        }
        ;

primary_expression              // The smallest expressions, need not have a l_value
      
    	: IDENTIFIER{
    		if(!stable->checkScope($1)){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", Undeclared variable '"<<$1<<"' "<<endl;
				exit(0);			
			}
			
    		$$ = new Identifier($1);
    		$$->type = stable->getType($1);
    		isConstant = 0;
    	}           // primary expression has IDENTIFIER now
	    | INT_CONSTANT{
	    	pusharraysize = $1;
	    	$$ = new IntConst($1);
	    	$$->type = new Type(Base, Int);
	    	isConstant = 1;
	    }
	    | FLOAT_CONSTANT{
	    	pusharraysize = $1;
	    	$$ = new FloatConst($1);
	    	$$->type = new Type(Base, Float);
	    	isConstant = 0;
	    }
	    | STRING_LITERAL{
	    	$$ = new StringConst($1);
	    	$$->type = new Type(Base, Void);
	    	isConstant = 0;
	    }
	    | '(' expression ')'{
	    	$$ = $2;
	    	isConstant = 0;
	    }
	    ;

compound_statement
		: '{' '}'{
			$$ = new BlockStmt();
			$$->type = new Type(Ok);
		}
		| '{' statement_list '}'{
			$$ = $2;
		}

	    | '{' declaration_list statement_list '}'{
	    	$$ = $3;
	    }
		;

statement_list
		: statement{
			$$ = new BlockStmt($1);
		}	
	    | statement_list statement{
	    	((BlockStmt*)$1)->children.push_back($2);
	    	$$ = $1;
	    }
		;

statement
	    : '{' statement_list '}'{
	    	$$ = $2;
	    }
	    | selection_statement{
	    	$$ = $1;
	    }
	    | iteration_statement{
	    	$$ = $1;
	    }	
		| assignment_statement{
			$$ = $1;
		}
	    | RETURN expression ';'{

	    	$$ = new Return($2, stable->retType);
	    	if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", Incompatible return type."<<endl;	
				exit(0);		
			}
	    }
	    ;

assignment_statement
		: ';'{
			$$ = new Ass();	//type??
		}						
		|  expression ';'{
			$$ = new Ass($1);// type??
		}
		;

expression                                   //assignment expressions are right associative
        :  logical_or_expression{
        	$$ = $1;
        }
        |  unary_expression '=' expression{
        	if(($1->type->check == 1) || ($1->type->check == 3)){
        		cout<<"Error:: On line "<<d_scanner.lineNr()<<", Only * can appear on left of = "<<endl;	
				exit(0);
        	}
        	//cout << $1->type->getType() << "  "<< $3->type->getType() << endl;
        	$$ = new Assign($1, $3);
        	if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", Assignment of Incompatible types."<<endl;	
				exit(0);		
			}
        }   // l_expression has been replaced by unary_expression.
        ;                                    // This may generate programs that are syntactically incorrect.
                                             // Eliminate them during semantic analysis.

logical_or_expression            // The usual hierarchy that starts here...
		: logical_and_expression{
			$$ = $1;
		}
	    | logical_or_expression OR_OP logical_and_expression{
	    	$$ = new OpBinary($1, $3, opNameB::OR);
	    	if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", OR operation on Incompatible types."<<endl;	
				exit(0);		
			}
	    }
		;
logical_and_expression
	    : equality_expression{
	    	$$ = $1;
	    }
	    | logical_and_expression AND_OP equality_expression{
	    	$$ = new OpBinary($1, $3, opNameB::AND);
	    	if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", AND operation on Incompatible types."<<endl;	
				exit(0);		
			}
	    }
		;

equality_expression
		: relational_expression{
			$$ = $1;
		}
	    | equality_expression EQ_OP relational_expression{
	    	$$ = new OpBinary($1, $3, opNameB::EQ_OP);
	    	if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", \'==\' operation on Incompatible types."<<endl;	
				exit(0);		
			}
	    }
		| equality_expression NE_OP relational_expression{
			$$ = new OpBinary($1, $3, opNameB::NE_OP);
			if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", \'!=\' operation on Incompatible types."<<endl;	
				exit(0);		
			}
		}
		;
relational_expression
		: additive_expression{
			$$ = $1;
		}
        | relational_expression '<' additive_expression{
        	$$ = new OpBinary($1, $3, opNameB::LT);
        	if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", \'<\' operation on Incompatible types."<<endl;	
				exit(0);		
			}

        } 
		| relational_expression '>' additive_expression{
			$$ = new OpBinary($1, $3, opNameB::GT);
			if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", \'>\' operation on Incompatible types."<<endl;	
				exit(0);		
			}

		}
		| relational_expression LE_OP additive_expression{
			$$ = new OpBinary($1, $3, opNameB::LE_OP);
			if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", \'<=\' operation on Incompatible types."<<endl;	
				exit(0);		
			}

		}
        | relational_expression GE_OP additive_expression{
        	$$ = new OpBinary($1, $3, opNameB::GE_OP);
        	if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", \'>=\' operation on Incompatible types."<<endl;	
				exit(0);		
			}

        } 
		;

additive_expression 
		: multiplicative_expression{
			$$ = $1;
		}
		| additive_expression '+' multiplicative_expression{
			$$ = new OpBinary($1, $3, opNameB::PLUS);
			if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", \'+\' operation on Incompatible types."<<endl;	
				exit(0);		
			}
		}
		| additive_expression '-' multiplicative_expression{
			$$ = new OpBinary($1, $3, opNameB::MINUS);
			if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", \'-\' operation on Incompatible types."<<endl;	
				exit(0);		
			}
		}
		;

multiplicative_expression
		: unary_expression{
			$$ = $1;
		}
		| multiplicative_expression '*' unary_expression{
			$$ = new OpBinary($1, $3, opNameB::MULT);
			if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", \'*\' operation on Incompatible types."<<endl;	
				exit(0);		
			}
		}
		| multiplicative_expression '/' unary_expression{
			$$ = new OpBinary($1, $3, opNameB::DIV);
			if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", \'/\' operation on Incompatible types."<<endl;	
				exit(0);		
			}
		}
		;

//done till here


unary_expression
		: postfix_expression{
			$$ = $1;
		}			
		| unary_operator unary_expression{
			$$ = new OpUnary($2, (OpUnary*)$1);
			if ($$->type->typeKind == Error){
				cout<<"Error:: On line "<<d_scanner.lineNr()<<", Incompatible unary operation."<<endl;	
				exit(0);		
			}
		}  // unary_operator can only be '*' on the LHS of '='
		;                                     // you have to enforce this during semantic analysis

postfix_expression
		: primary_expression{
			//cout << $1->type->getType() << " " <<$1->type->num_type_pointers<< " "<< $1->type->isArray<<endl;
			$$ = $1;
		}  				
        | IDENTIFIER '(' ')'{
        	$$ = new Funcall(new Identifier($1));
        	if (gtable->funsymtable.find($1) == gtable->funsymtable.end()) {
				if($1 != stable->entryName){
					cout << "Error:: On line " << d_scanner.lineNr() << " Function " << $1 << " is not defined." << endl;
					exit(0);
				}
			}

			else {
				SymbolTable*calledst;
				if($1 == stable->entryName){
					calledst = stable;
				}
				else {
					calledst = gtable->funsymtable[$1];
				}
				if (calledst->parameters.size() != 0){
					cout << "Error:: On line " << d_scanner.lineNr() << " Function " << $1 << " has " << calledst->parameters.size() << " parameters, " << " 0 given." << endl;
					exit(0);
				}
			}
			//valid function call
			if($1 == stable->entryName){
					$$->type = stable->retType;
				}
				else {
					$$->type = gtable->funsymtable[$1]->retType;
				}
				$$->type->check = 3;
        } 				    // Cannot appear on the LHS of '='. Enforce this.



	    | IDENTIFIER '(' expression_list ')'{
	    	((Funcall*)$3)->children.insert(((Funcall*)$3)->children.begin(), new Identifier($1));
			
			$$ = $3;
			Funcall *fc = (Funcall *) $$;
			if (gtable->funsymtable.find($1) == gtable->funsymtable.end()) {
				if($1 != stable->entryName){
					cout << "Error:: On line " << d_scanner.lineNr() << " Function " << $1 << " is not defined." << endl;
					exit(0);
				}
			}
			SymbolTable*calledst;
			if($1 == stable->entryName){
				calledst = stable;
			}
			else {
				calledst = gtable->funsymtable[$1];
			}

			if (fc->children.size() - 1 != calledst->parameters.size()){
				cout << "Error:: On line " << d_scanner.lineNr() << " Function " << $1 << " has " << calledst->parameters.size() << " parameters, " << fc->children.size() - 1 << " given." << endl;
				exit(0);
			}
			else{
				for (int i = 1; i < fc->children.size(); i++){
					Type * actualtype = calledst->getParaByInd(i-1);
					cout << fc->children[i]->type->getType()<<fc->children[i]->type->isArray<<fc->children[i]->type->num_type_pointers<<fc->children[i]->type->typeKind<<endl;
					cout << actualtype->getType()<<actualtype->isArray<<actualtype->num_type_pointers<<actualtype->typeKind<<endl;

					if(!(fc->children[i]->type)->equal(actualtype)){
						//cout << fc->children[i]->type->getType()<<endl;
						if(fc->children[i]->type->typeKind == Base && actualtype->typeKind == Base){
							if (fc->children[i]->type->base == Int) {
								OpUnary *xf = new OpUnary(fc->children[i], TO_FLOAT);
								fc->children[i] = xf;
							}
							else if(fc->children[i]->type->base == Float) {
								OpUnary *xf = new OpUnary(fc->children[i], TO_INT);
								fc->children[i] = xf;
							}
							else{
								cout << "Error:: On line " << d_scanner.lineNr() << " Parameter " << i << " of Function " << $1 << " has wrong type." << endl;
								exit(0);
							}

						}
						else{
							cout << "Error:: On line " << d_scanner.lineNr() << " Parameter " << i << " of Function " << $1 << " has wrong type." << endl;
							exit(0);
						}
					}


				}
				//Valid parameters
				if($1 == stable->entryName){
					$$->type = stable->retType;
				}
				else {
					$$->type = gtable->funsymtable[$1]->retType;
				}
				$$->type->check = 3;

			}
	    }    // Cannot appear on the LHS of '='  Enforce this.



        | postfix_expression '[' expression ']'{
        	
        	if(!($3->type->typeKind == Base)&&($3->type->base == Int)){
        		cout << "Error:: On line " << d_scanner.lineNr() << " Array subscript is not an integer" << endl;
				exit(0);
        	}
        	Type *t = $1->type;
        	$$ = new ArrayRef($1, $3);
        	$$->type = t;

        	if($$->type->typeKind != Pointer){
        		cout << "Error:: On line " << d_scanner.lineNr() << " subscripted value is neither array nor pointer" << endl;
				exit(0);
        	}

        	$$->type->num_type_pointers--;
        	if($$->type->num_type_pointers == 0){
        		$$->type->typeKind = Base;
        	}
        }
        | postfix_expression '.' IDENTIFIER{
        	if(!(($1->type->typeKind == Base)&&($1->type->base == Struct))){
        		cout << "Error:: On line " << d_scanner.lineNr() << "  request for member \'"<<$3 <<"\' in something not a structure" << endl;
				exit(0);
        	}
        	map <string, SymbolTable*>::iterator it = gtable->strsymtable.find($1->type->structType);
        	map <string, SymbolTableEntry*>::iterator pit = it->second->localvars.find($3);
        	if(pit == it->second->localvars.end()){
        		cout << "Error:: On line " << d_scanner.lineNr() << "  Struct \'"<<$1->type->structType <<"\' has no member named \'" <<$3<<"\'" << endl;
				exit(0);
        	}
        	$$ = new Member($1, new Identifier($3));
        	$$->type = pit->second->idType->copy();
        }

        | postfix_expression PTR_OP IDENTIFIER{
        	$$ = new Arrow($1, new Identifier($3));
        	if(!(($1->type->typeKind == Pointer)&&($1->type->base == Struct))){
        		cout << "Error:: On line " << d_scanner.lineNr() << "  request for member \'"<<$3 <<"\' in something not a structure" << endl;
				exit(0);
        	}
        	map <string, SymbolTable*>::iterator it = gtable->strsymtable.find($1->type->structType);
        	map <string, SymbolTableEntry*>::iterator pit = it->second->localvars.find(ptable->name);
        	if(pit == it->second->localvars.end()){
        		cout << "Error:: On line " << d_scanner.lineNr() << "  Struct \'"<<$1->type->structType <<"\' has no member named \'" <<$3<<"\'" << endl;
				exit(0);
        	}
        	if($1->type->num_type_pointers != 1){
        		cout << "Error:: On line " << d_scanner.lineNr() << "  request for member \'"<<$3 <<"\' in something not a structure" << endl;
				exit(0);
        	}
        	$$->type = pit->second->idType->copy();
        }

	    | postfix_expression INC_OP{
	    	$$ = new OpUnary($1, opNameU::PP);
	    } 	       // Cannot appear on the LHS of '='   Enforce this
		;

// There used to be a set of productions for l_expression at this point.

expression_list
        : expression{
        	//cout << " bkenf "<< $1->type->getType()<<$1->type->isArray<<$1->type->num_type_pointers<<$1->type->typeKind<<endl;
        	$$ = new Funcall($1);
        }
        | expression_list ',' expression{
       		// cout << " bkenf "<< $3->type->getType()<<$3->type->isArray<<$3->type->num_type_pointers<<$3->type->typeKind<<endl;
        	((Funcall*)$1)->children.push_back($3);
			$$ = $1;
        }
		;

unary_operator
        : '-'{
        	$$ = new OpUnary(opNameU::UMINUS);
        }
		| '!'{
			$$ = new OpUnary(opNameU::NOT);
		}
        | '&'{
        	$$ = new OpUnary(opNameU::DEREF);
        }
        | '*'{
        	$$ = new OpUnary(opNameU::POINTER);
        }
		;

selection_statement
        : IF '(' expression ')' statement ELSE statement{
        	$$ = new If($3, $5, $7);
        }
		;

iteration_statement
		: WHILE '(' expression ')' statement{
			$$ = new While($3, $5);
		}
		| FOR '(' expression ';' expression ';' expression ')' statement{
			$$ = new For($3, $5, $7, $9);
		}  //modified this production
        ;

declaration_list
        : declaration
        {	
        	stable->localvars = paraMap;
        }  					
        | declaration_list declaration
        {
        	stable->localvars = paraMap;
        }
		;

declaration
		: type_specifier
		{	
			ptable = new SymbolTableEntry();
			ptable->idType = retType;
			ptable->numPointers = 0;
			ptable->isArray = 0;

		}
		declarator_list';' 
		;

declarator_list
		: declarator
		{	
			if(ptable->idType->base == Void){
				if(ptable->idType->typeKind != Pointer){
					cout << "Error:: On line " << d_scanner.lineNr() <<" variable or field "<< " \'"<< ptable->name <<"\' declared void"  << endl;
					exit(0);
				}
			}
			if(ptable->idType->base == Struct){
				map <string, SymbolTable*>::iterator it = gtable->strsymtable.find(ptable->idType->structType);
				if(it == gtable->strsymtable.end()){
					//put the comments to let undeclared struct pointer declare
						if(ptable->numPointers == 0){
							cout << "Error:: On line " << d_scanner.lineNr() << " \'"<< ptable->name <<"\' has incomplete type"  << endl;
							exit(0);
						}
				}
				else{
					if((ptable->idType->structType == stable->entryName) && (stable->isStruct == 1)){
						if(ptable->numPointers == 0){
						cout << "Error:: On line " << d_scanner.lineNr() << " \'"<< ptable->name <<"\' has incomplete type"  << endl;
						exit(0);
						}
					}
				}
			}

			map <string, SymbolTableEntry*>::iterator pit = paraMap.find(ptable->name);
			if(pit != paraMap.end()){
					string tmp = pit->second->idType->getType();
				 	int t;
					if(pit->second->isArray == 1){
						t = pit->second->numPointers - pit->second->arrayVector.size();
					}
				 	for(int i = 0; i < t; i++){
				 		tmp = tmp + "*";
				 	}
					cout << "Error:: On line " << d_scanner.lineNr() << " '"<< ptable->name <<"\' has a previous declaration as \'"<< tmp << " " <<ptable->name << "\'"<< endl;
					exit(0);
			}
			pit = stable->parameters.find(ptable->name);
			if(pit != stable->parameters.end()){
					string tmp = pit->second->idType->getType();
				 	int t;
					if(pit->second->isArray == 1){
						t = pit->second->numPointers - pit->second->arrayVector.size();
					}
				 	for(int i = 0; i < t; i++){
				 		tmp = tmp + "*";
				 	}
					cout << "Error:: On line " << d_scanner.lineNr() << " '"<< ptable->name <<"\' has a previous declaration as \'"<< tmp << " " <<ptable->name << "\'"<< endl;
					exit(0);
			}




			paraMap[ptable->name] = ptable;
			stable->localvars = paraMap;
			offset -= ptable->size();
			ptable->offset = offset;
		}
		| declarator_list ','
		{
			ptable = new SymbolTableEntry();
			ptable->idType = retType;
			ptable->isArray = 0;
			ptable->numPointers = 0;
		} 
		declarator 
		{
			if(ptable->idType->base == Void){
				if(ptable->idType->typeKind != Pointer){
					cout << "Error:: On line " << d_scanner.lineNr() <<" variable or field "<< " \'"<< ptable->name <<"\' declared void"  << endl;
					exit(0);
				}
			}
			if(ptable->idType->base == Struct){
				map <string, SymbolTable*>::iterator it = gtable->strsymtable.find(ptable->idType->structType);
				if(it == gtable->strsymtable.end()){
					//put the comments to let undeclared struct pointer declare
						if(ptable->numPointers == 0){
							cout << "Error:: On line " << d_scanner.lineNr() << " \'"<< ptable->name <<"\' has incomplete type"  << endl;
							exit(0);
						}
				}
				else{
					if((ptable->idType->structType == stable->entryName) && (stable->isStruct == 1)){
						if(ptable->numPointers == 0){
						cout << "Error:: On line " << d_scanner.lineNr() << " \'"<< ptable->name <<"\' has incomplete type"  << endl;
						exit(0);
						}
					}
				}
			}

			map <string, SymbolTableEntry*>::iterator pit = paraMap.find(ptable->name);
			if(pit != paraMap.end()){
					string tmp = pit->second->idType->getType();
					int t;
					if(pit->second->isArray == 1){
						t = pit->second->numPointers - pit->second->arrayVector.size();
					}
				 	for(int i = 0; i < t; i++){
				 		tmp = tmp + "*";
				 	}
					cout << "Error:: On line " << d_scanner.lineNr() << " '"<< ptable->name <<"\' has a previous declaration as \'"<< tmp << " " <<ptable->name << "\'"<< endl;
					exit(0);

			}
			pit = stable->parameters.find(ptable->name);
			if(pit != stable->parameters.end()){
					string tmp = pit->second->idType->getType();
				 	int t;
					if(pit->second->isArray == 1){
						t = pit->second->numPointers - pit->second->arrayVector.size();
					}
				 	for(int i = 0; i < t; i++){
				 		tmp = tmp + "*";
				 	}
					cout << "Error:: On line " << d_scanner.lineNr() << " '"<< ptable->name <<"\' has a previous declaration as \'"<< tmp << " " <<ptable->name << "\'"<< endl;
					exit(0);
			}

			paraMap[ptable->name] = ptable;
			stable->localvars = paraMap;
			offset -= ptable->size();
			ptable->offset = offset;
		}
	 	;