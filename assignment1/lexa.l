// There may be some flexer directives here
%%
[ \t]                                    //skip

void									return VOID;
int										return INT;
float									return FLOAT;
"("                                     return '(';
")"                                     return ')';
","                                     return ',';
"["                                     return '[';
"]"                                     return ']';
"{"                                     return '{';
"}"                                     return '}';
";"                                     return ';';
return 									return RETURN;
(-?)[[:digit:]]+ 						return INT_CONSTANT;
// FLOAT_CONSTANT
// STRING_LITERAL
"||"									return OR_OP;
"&&"									return AND_OP;
"=="									return EQ_OP;
"!="									return NE_OP;
"<="									return LE_OP;
">="									return GE_OP;
"++"									return INC_OP;
"="                                     return '=';
"<"                                     return '<';
">"                                     return '>';
"+"                                     return '+';
"-"                                     return '-';
"*"                                     return '*';
"/"                                     return '/';
"!"                                     return '!';
if 										return IF;
while 									return WHILE;
for										return FOR;
[[:alpha:]][[:alnum:]]*					return IDENTIFIER;
