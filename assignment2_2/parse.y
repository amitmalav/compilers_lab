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
        : STRUCT IDENTIFIER
        {
        	stable = new SymbolTable();
        	stable->entryName = $2;
        	stable->isStruct = 1;
        	stable->retType = new Type(Kind::Base, Basetype::Struct, $2);
        	offset = 0;
        } '{' declaration_list '}' ';'
        {
        	stable->localvars = paraMap;
        	gtable->insertTable(stable);
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
		} 
		fun_declarator
		{
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
        	gtable->insertTable(stable);
        	paraMap.clear();
        	//$$->print();
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
			ptable = new SymbolTableEntry();
			ptable->idType = $1;
			ptable->numPointers = 0;
			ptable->isArray = 0;
		}
		declarator
		{
			offset += ptable->size();
			ptable->offset = offset;
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
			ptable->isArray = 1;
			ptable->arrayVector.push_back((int)pusharraysize);
		} // check separately that it is a constant //  yet to be done
        | '*'
        {
	    	ptable->numPointers +=1;
	    	ptable->idType->typeKind = Pointer;
	    }
        declarator
        {
        	$$ = $3;
        }
        ;

primary_expression              // The smallest expressions, need not have a l_value
      
    	: IDENTIFIER{
    	$$ = new Identifier($1);
    	}           // primary expression has IDENTIFIER now
	    | INT_CONSTANT{
	    	pusharraysize = $1;
	    	$$ = new IntConst($1);
	    }
	    | FLOAT_CONSTANT{
	    	pusharraysize = $1;
	    	$$ = new FloatConst($1);
	    }
	    | STRING_LITERAL{
	    	$$ = new StringConst($1);
	    }
	    | '(' expression ')'{
	    	$$ = $2;
	    }
	    ;

compound_statement
		: '{' '}'{
			$$ = new BlockStmt();
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
	    	$$ = new Return($2);
	    }
	    ;

assignment_statement
		: ';'{
			$$ = new Ass();
		}						
		|  expression ';'{
			$$ = new Ass($1);
		}
		;

expression                                   //assignment expressions are right associative
        :  logical_or_expression{
        	$$ = $1;
        }
        |  unary_expression '=' expression{
        	$$ = new Assign($1, $3);
        }   // l_expression has been replaced by unary_expression.
        ;                                    // This may generate programs that are syntactically incorrect.
                                             // Eliminate them during semantic analysis.

logical_or_expression            // The usual hierarchy that starts here...
		: logical_and_expression{
			$$ = $1;
		}
	    | logical_or_expression OR_OP logical_and_expression{
	    	$$ = new OpBinary($1, $3, opNameB::OR);
	    }
		;
logical_and_expression
	    : equality_expression{
	    	$$ = $1;
	    }
	    | logical_and_expression AND_OP equality_expression{
	    	$$ = new OpBinary($1, $3, opNameB::AND);
	    } 
		;

equality_expression
		: relational_expression{
			$$ = $1;
		}
	    | equality_expression EQ_OP relational_expression{
	    	$$ = new OpBinary($1, $3, opNameB::EQ_OP);
	    }
		| equality_expression NE_OP relational_expression{
			$$ = new OpBinary($1, $3, opNameB::NE_OP);
		}
		;
relational_expression
		: additive_expression{
			$$ = $1;
		}
        | relational_expression '<' additive_expression{
        	$$ = new OpBinary($1, $3, opNameB::LT);
        } 
		| relational_expression '>' additive_expression{
			$$ = new OpBinary($1, $3, opNameB::GT);
		}
		| relational_expression LE_OP additive_expression{
			$$ = new OpBinary($1, $3, opNameB::LE_OP);
		}
        | relational_expression GE_OP additive_expression{
        	$$ = new OpBinary($1, $3, opNameB::GE_OP);
        } 
		;

additive_expression 
		: multiplicative_expression{
			$$ = $1;
		}
		| additive_expression '+' multiplicative_expression{
			$$ = new OpBinary($1, $3, opNameB::PLUS);
		}
		| additive_expression '-' multiplicative_expression{
			$$ = new OpBinary($1, $3, opNameB::MINUS);
		}
		;

multiplicative_expression
		: unary_expression{
			$$ = $1;
		}
		| multiplicative_expression '*' unary_expression{
			$$ = new OpBinary($1, $3, opNameB::MULT);
		}
		| multiplicative_expression '/' unary_expression{
			$$ = new OpBinary($1, $3, opNameB::MULT);
		}
		;
unary_expression
		: postfix_expression{
			$$ = $1;
		}			
		| unary_operator unary_expression{
			$$ = new OpUnary($2, (OpUnary*)$1);
		}  // unary_operator can only be '*' on the LHS of '='
		;                                     // you have to enforce this during semantic analysis

postfix_expression
		: primary_expression{
			$$ = $1;
		}  				
        | IDENTIFIER '(' ')'{
        	$$ = new Funcall(new Identifier($1));
        } 				    // Cannot appear on the LHS of '='. Enforce this.
	    | IDENTIFIER '(' expression_list ')'{
	    	((Funcall*)$3)->children.insert(((Funcall*)$3)->children.begin(), new Identifier($1));
			$$ = $3;
	    }    // Cannot appear on the LHS of '='  Enforce this.
        | postfix_expression '[' expression ']'{
        	$$ = new ArrayRef($1, $3);
        }
        | postfix_expression '.' IDENTIFIER{
        	$$ = new Member($1, new Identifier($3));
        }
        | postfix_expression PTR_OP IDENTIFIER{
        	$$ = new Arrow($1, new Identifier($3));
        }
	    | postfix_expression INC_OP{
	    	$$ = new OpUnary($1, opNameU::PP);
	    } 	       // Cannot appear on the LHS of '='   Enforce this
		;

// There used to be a set of productions for l_expression at this point.

expression_list
        : expression{
        	$$ = new Funcall($1);
        }
        | expression_list ',' expression{
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
			paraMap[ptable->name] = ptable;
			stable->localvars = paraMap;
			offset -= ptable->size();
			ptable->offset = offset;
		}
	 	;


