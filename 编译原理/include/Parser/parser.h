#pragma once
#include "Lexer/lexer.h"
#include "Symbols/symbols.h"
#include "Inter/stmt.h"

namespace Parser {
	class Parser {
	private:
		Lexer::Lexer& lexer;
		shared_ptr<Lexer::Token> look;
		Symbols::Env top; // 当前或顶部的符号表
		int used = 0; // storage used for declarations

	public:
		Parser(Lexer::Lexer& l);

		// 获取下一个词法单元到 look
		void move();

		void error(std::string s);

		void match(int t);

		void program();

		
	};
}