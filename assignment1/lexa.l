// There may be some flexer directives here
exponent-part								([eE][+-][0-9]*)|([eE][0-9]*)
floating-point-constant						[+-]?([0-9]+\.[0-9]*{exponent-part}?)|([0-9]*\.[0-9]+{exponent-part}?)
%%
[ \t]                                    //skip


\"([^\\]|([\\][tn\"\\]))*\"				return STRING_LITERAL;
void									return VOID;
int										return INT;
float									return FLOAT;
return 									return RETURN;
"||"									return OR_OP;
"&&"									return AND_OP;
"=="									return EQ_OP;
"!="									return NE_OP;
"<="									return LE_OP;
">="									return GE_OP;
"++"									return INC_OP;
if 										return IF;
else									return ELSE;
while 									return WHILE;
for										return FOR;
(-?)[[:digit:]]+ 						return INT_CONSTANT;
{floating-point-constant}				return FLOAT_CONSTANT;
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
[[:alpha:]][[:alnum:]]*					return IDENTIFIER;
.										return OTHER;