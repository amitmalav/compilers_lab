#include <iostream>
#include "Scanner.h"
using namespace std;
int main() {
	Scanner scanner;   // define a Scanner object
	while (int token = scanner.lex()) // get all tokens
	{
		string const &text = scanner.matched();
		switch (token) {
		case Scanner::VOID:
			cout << "VOID: " << text << '\n';
			break;
		case Scanner::INT:
			cout << "INT: " << text << '\n';
			break;
		case Scanner::FLOAT:
			cout << "FLOAT: " << text << '\n';
			break;
		case Scanner::RETURN:
			cout << "RETURN: " << text << '\n';
			break;
		case Scanner::INT_CONSTANT:
			cout << "INT_CONSTANT: " << text << '\n';
			break;
		case Scanner::FLOAT_CONSTANT:
			cout << "FLOAT_CONSTANT: " << text << '\n';
			break;
		case Scanner::STRING_LITERAL:
			cout << "STRING_LITERAL: " << text << '\n';
			break;
		case Scanner::OR_OP:
            cout << "OR_OPERATOR: " << text << '\n';
			break;
		case Scanner::AND_OP:
            cout << "AND_OPERATOR: " << text << '\n';
			break;
		case Scanner::EQ_OP:
            cout << "EQ_OPERATOR: " << text << '\n';
			break;
		case Scanner::NE_OP:
            cout << "NE_OPERATOR: " << text << '\n';
			break;
		case Scanner::INC_OP:
            cout << "INC_OPERATOR: " << text << '\n';
			break;
		case Scanner::LE_OP:
            cout << "LESS_OR_EQ_OPERATOR: " << text << '\n';
			break;
		case Scanner::GE_OP:
            cout << "GREATER_OR_EQ_OPERATOR: " << text << '\n';
			break;
		case Scanner::IF:
            cout << "IF: " << text << '\n';
			break;
		case Scanner::ELSE:
            cout << "ELSE: " << text << '\n';
			break;
		case Scanner::WHILE:
            cout << "WHILE: " << text << '\n';
			break;
		case Scanner::FOR:
            cout << "FOR: " << text << '\n';
			break;
		case Scanner::IDENTIFIER:
            cout << "IDENTIFIER: " << text << '\n';
			break;
		case '+':
			cout << "plus: " << text << '\n';
			break;
		case ',':
			cout << "comma: " << text << '\n';
			break;
		case '(':
			cout << "lparen: " << text << '\n';
			break;
		case ')':
			cout << "rparen: " << text << '\n';
			break;
		case ';':
			cout << "semi-colon: " << text << '\n';
			break;
		case '[':
			cout << "lrectbrac: " << text << '\n';
			break;
		case ']':
			cout << "rrectbrac: " << text << '\n';
			break;
		case '{':
			cout << "lflowerbrac: " << text << '\n';
			break;
		case '}':
			cout << "rflowerbrac: " << text << '\n';
			break;
		case '-':
			cout << "minus: " << text << '\n';
			break;
		case '*':
			cout << "multiply: " << text << '\n';
			break;
		case '/':
			cout << "divide: " << text << '\n';
			break;
		case '!':
			cout << "not: " << text << '\n';
			break;
		case '=':
			cout << "assign: " << text << '\n';
			break;
		case '<':
			cout << "less_than: " << text << '\n';
			break;
		case '>':
			cout << "greater_than: " << text << '\n';
			break;
		default:
		  	cout << "char. token: `" << text << "'\n";
		}
	}
}







/*#include <iostream>
#include "Scanner.h"
#include "Parser.h"
using namespace std;
int main(int argc, char** arg) {
	Parser parser;
	parser.parse();
}*/

