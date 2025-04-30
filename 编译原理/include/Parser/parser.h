#pragma once
#include "Lexer/lexer.h"
#include "Symbols/symbols.h"
#include "Inter/stmt.h"
#include<stack>

namespace Lexer {
	class Lexer;
}

namespace Symbols {
	class Type;
}

namespace Inter {
	class Stmt;
	class Expr;
	class Op;
	class Access;
	class Id;
	class Node;
}

namespace Parser {
	class Parser {
	private:
		Lexer::Lexer& lexer;
		shared_ptr<Lexer::Token> look;
		std::stack<int> stateStack;
		Symbols::Env top; // 当前或顶部的符号表
		int used = 0; // storage used for declarations

	public:
		Parser(Lexer::Lexer& l);

		// 获取下一个词法单元到 look
		void move();

		void error(std::string s);

		void match(int t);

		// program -> block
		void program(); 

		// block -> { decls stmts }
		shared_ptr<Inter::Stmt> block();

		void decls();

		shared_ptr<Symbols::Type> type();

		shared_ptr<Symbols::Type> dims(std::shared_ptr<Symbols::Type> p);

		shared_ptr<Inter::Stmt> stmts();

		shared_ptr<Inter::Stmt> stmt();

		shared_ptr<Inter::Stmt> assign();

		shared_ptr<Inter::Expr> bool_();

		shared_ptr<Inter::Expr> join();

		shared_ptr<Inter::Expr> equality();

		shared_ptr<Inter::Expr> rel();

		shared_ptr<Inter::Expr> expr();

		shared_ptr<Inter::Expr> term();

		shared_ptr<Inter::Expr> unary();

		shared_ptr<Inter::Expr> factor();

		shared_ptr<Inter::Access> offset(shared_ptr<Inter::Id> a);
	};
}