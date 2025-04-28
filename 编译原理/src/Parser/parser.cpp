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
	shared_ptr<Inter::Stmt> Parser::block()
	{
		return shared_ptr<Inter::Stmt>();
	}
	void Parser::decls()
	{
	}
	shared_ptr<Symbols::Type> Parser::type()
	{
		return shared_ptr<Symbols::Type>();
	}
	shared_ptr<Symbols::Type> Parser::dims(std::shared_ptr<Symbols::Type> p)
	{
		return shared_ptr<Symbols::Type>();
	}
	shared_ptr<Inter::Stmt> Parser::stmts()
	{
		return shared_ptr<Inter::Stmt>();
	}
	shared_ptr<Inter::Stmt> Parser::stmt()
	{
		return shared_ptr<Inter::Stmt>();
	}
	shared_ptr<Inter::Stmt> Parser::assign()
	{
		return shared_ptr<Inter::Stmt>();
	}
	shared_ptr<Inter::Expr> Parser::bool_()
	{
		return shared_ptr<Inter::Expr>();
	}
	shared_ptr<Inter::Expr> Parser::join()
	{
		return shared_ptr<Inter::Expr>();
	}
	shared_ptr<Inter::Expr> Parser::equality()
	{
		return shared_ptr<Inter::Expr>();
	}
	shared_ptr<Inter::Expr> Parser::rel()
	{
		return shared_ptr<Inter::Expr>();
	}
	shared_ptr<Inter::Expr> Parser::expr()
	{
		return shared_ptr<Inter::Expr>();
	}
	shared_ptr<Inter::Expr> Parser::term()
	{
		return shared_ptr<Inter::Expr>();
	}
	shared_ptr<Inter::Expr> Parser::unary()
	{
		return shared_ptr<Inter::Expr>();
	}
	shared_ptr<Inter::Expr> Parser::factor()
	{
		return shared_ptr<Inter::Expr>();
	}
	shared_ptr<Inter::Access> Parser::offset(shared_ptr<Inter::Id> a)
	{
		return shared_ptr<Inter::Access>();
	}
}

