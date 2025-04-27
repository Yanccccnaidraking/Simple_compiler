#include "Parser/parser.h"
#include "Lexer/lexer.h"
namespace Parser {
	Parser::Parser(Lexer::Lexer& l) : lexer(l), top(nullptr)
	{
	}
	void Parser::move()
	{
	}
	void Parser::error(std::string s)
	{
	}
	void Parser::match(int t)
	{
	}
	void Parser::program()
	{
	}
}

