%debug
%scanner Scanner.h
%scanner-token-function d_scanner.lex()
%token STRUCT VOID INT FLOAT RETURN INT_CONSTANT FLOAT_CONSTANT STRING_LITERAL OR_OP AND_OP EQ_OP NE_OP LE_OP GE_OP INC_OP PTR_OP IF ELSE WHILE FOR IDENTIFIER OTHER

%polymorphic StmtAst : StmtAst*; ExpAst : ExpAst*; RefAst : RefAst*; Int : int; Float : float; String : std::string;

%type <StmtAst> translation_unit function_definition struct_specifier compound_statement statement_list statement assignment_statement selection_statement iteration_statement

%type <ExpAst> constant_expression expression logical_and_expression equality_expression relational_expression additive_expression multiplicative_expression unary_expression postfix_expression primary_expression expression_list unary_operator

%type <RefAst> l_expression

%type <Int> INT_CONSTANT

%type <Float> FLOAT_CONSTANT

%type <String>	STRING_LITERAL IDENTIFIER  

%%

translation_unit 
        :  struct_specifier{
        }
 		| function_definition{
 			$$ = $1;
 			//$$->print();
 		}
 		| translation_unit function_definition{
 			std::vector<StmtAst*> seqVector;
 			seqVector.push_back($1);
 			seqVector.push_back($2);
 			$$ = new Seq(seqVector);
 			//$$->print();
 		}
        | translation_unit struct_specifier{
        /*
 			std::vector<StmtAst*> seqVector;
 			seqVector.push_back($1);
 			seqVector.push_back($2);
 		*/
 			$$ = new Seq($1);
 			//$$->print();
 		}
        ;

struct_specifier 
        : STRUCT IDENTIFIER '{' declaration_list '}' ';'{
        }
        ;

function_definition
		: type_specifier fun_declarator compound_statement{
        	$$ = $3;
        	$$->print();
        }
		;

type_specifier
        : base_type
        | type_specifier '*'
        ;

base_type 
        : VOID 	
        | INT   
        | FLOAT 
        | STRUCT IDENTIFIER 
        ;

fun_declarator
		: IDENTIFIER '(' parameter_list ')' 
		| IDENTIFIER '(' ')' 
		;

parameter_list
		: parameter_declaration 
		| parameter_list ',' parameter_declaration 
		;

parameter_declaration
		: type_specifier declarator 
        ;

declarator
		: IDENTIFIER 
		| declarator '[' constant_expression ']' 
        ;

constant_expression 
        : INT_CONSTANT{
        	$$ = new IntConst($1);	
        }
        | FLOAT_CONSTANT{
        	$$ = new FloatConst($1);
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
		|  l_expression '=' expression ';'{

			$$ = new Ass($1, $3);
		}
		;

expression
		: logical_and_expression{
			$$ = $1;
		}
	    | expression OR_OP logical_and_expression{
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
		| unary_operator postfix_expression{

			$$ = new OpUnary($1, (OpUnary*)$2);
		}
		;

postfix_expression
		: primary_expression{
			$$ = $1;
		}		
	    | IDENTIFIER '(' ')'{
	    	$$ = new Funcall(new Identifier($1));
	    }			
		| IDENTIFIER '(' expression_list ')'{
			((Funcall*)$3)->children.insert(((Funcall*)$3)->children.begin(), new Identifier($1));
			$$ = $3;
		}
		| l_expression INC_OP{
			$$ = new OpUnary($1, opNameU::PP);
		}
		;

primary_expression
		: l_expression{
			$$ = $1;
		}
        | l_expression '=' expression{

        	 $$ = new Assign($1, $3);
        } 
        | '&' l_expression{
        	$$ = new Pointer($2);
        }

        | INT_CONSTANT{

        	$$ = new IntConst($1);
        }
		| FLOAT_CONSTANT{
			$$ = new FloatConst($1);
		}
        | STRING_LITERAL{
        	$$ = new StringConst($1);
        }
		| '(' expression ')'{
			$$ = $2;
		}
		;

l_expression
        : IDENTIFIER{
        	$$ = new Identifier($1);

        }
        | l_expression '[' expression ']'{

        	$$ = new ArrayRef($1, $3); 
        }
        | '*' l_expression{
        	$$ = new DeRef($2);
        }
        | l_expression '.' IDENTIFIER{
        	$$ = new Member($1, new Identifier($3));
        }
        | l_expression PTR_OP IDENTIFIER{
        	$$ = new Arrow($1, new Identifier($3));
        }
        ;

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
		}
        ;

declaration_list
        : declaration  					
        | declaration_list declaration
		;

declaration
		: type_specifier declarator_list';' 
		;

declarator_list
		: declarator
		| declarator_list ',' declarator 
	 	;
