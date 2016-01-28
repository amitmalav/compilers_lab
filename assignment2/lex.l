// There may be some flexer directives here
exponent-part								([eE][+-][0-9]*)|([eE][0-9]*)
floating-point-constant						[+-]?([0-9]+\.[0-9]*{exponent-part}?)|([0-9]*\.[0-9]+{exponent-part}?)z
%%
[ \t\n]+                                    //skip
\"([^\\]|([\\][tn\"\\]))*\"				return Parser::STRING_LITERAL;
struct									return Parser::STRUCT;
void									return Parser::VOID;
int										return Parser::INT;
float									return Parser::FLOAT;
return 									return Parser::RETURN;
"||"									return Parser::OR_OP;
"&&"									return Parser::AND_OP;
"=="									return Parser::EQ_OP;
"!="									return Parser::NE_OP;
"<="									return Parser::LE_OP;
">="									return Parser::GE_OP;
"++"									return Parser::INC_OP;
"->"									return Parser::PTR_OP;
if 										return Parser::IF;
else									return Parser::ELSE;
while 									return Parser::WHILE;
for										return Parser::FOR;
(-?)[[:digit:]]+ 						return Parser::INT_CONSTANT;
{floating-point-constant}				return Parser::FLOAT_CONSTANT;
"("                                     return '(';
")"                                     return ')';
","                                     return ',';
"["                                     return '[';
"]"                                     return ']';
"{"                                     return '{';
"}"                                     return '}';
";"                                     return ';';
"="                                     return '=';
"<"                                     return '<';
">"                                     return '>';
"+"                                     return '+';
"-"                                     return '-';
"*"                                     return '*';
"/"                                     return '/';
"!"                                     return '!';
[[:alpha:]][[:alnum:]]*					return Parser::IDENTIFIER;
.										return Parser::OTHER;
