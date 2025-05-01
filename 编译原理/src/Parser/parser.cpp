#include "Parser/parser.h"
#include "Lexer/lexer.h"
#include "Inter/stmt.h"
#include "Lexer/Tag.h"
#include "Parser/lr_1.h"
namespace Parser {
	Parser::Parser(Lexer::Lexer& l) : lexer(l), top(nullptr)
	{
		deserializeItemSets("ItemSets.dat");
		//printItemSets();
		loadGotoTable("GOTOTable.dat");
		loadActionTable("ActionTable.dat");
		stateStack.push(0);
		move();
	}
	void Parser::move()
	{
		look = lexer.scan();
	}
	void Parser::error(std::string s)
	{
		throw runtime_error("line " + std::to_string(lexer.line) + ": " + s);
	}
	void Parser::match(int t)
	{
		if (look->tag == t) move();
		else error("语法错误");
	}
	void Parser::program()
	{
		bool finished = false;
		std::deque<std::string> symbols;
		std::string token = Lexer::tagToString(Lexer::Tag(look->tag));
		while (!finished)
		{
			int cur = stateStack.top();
			//cout << "栈顶的数值是：" << cur << endl;
			if (cur >= 0)
			{
				Action act = searchFromAction(cur, token);
				switch (act.type)
				{
				case ActionType::Shift:
					insertParserTable(stateStack, symbols, token, "移入");
					stateStack.push(act.value);
					move();
					symbols.push_back(token);
					token = Lexer::tagToString(Lexer::Tag(look->tag));
					break;
				case ActionType::Reduce:
				{
					insertParserTable(stateStack, symbols, token, "根据"+toStringProduction(grammar[act.value]) + "规约");
					int beta = grammar[act.value].right[0]==""? 0 :grammar[act.value].right.size();//空串这一特殊情况设置成0
					while (beta--)
					{
						stateStack.pop();
						symbols.pop_back();
					}
					int newState = searchFromGoto(stateStack.top(), grammar[act.value].left);
					if (newState == -1)
					{
						//错误处理
						break;
					}
					stateStack.push(newState);
					symbols.push_back(grammar[act.value].left);
					//输出产生式（待做...）
					break;
				}
				case ActionType::Accept:
					insertParserTable(stateStack, symbols, token, "接收");
					finished = true;//语法分析结束
					break;
				case ActionType::Error:
					//调用错误恢复例程
					error("语法分析错误");
					break;
				}
			}
			else
			{
				break;
			}
		}
		
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

