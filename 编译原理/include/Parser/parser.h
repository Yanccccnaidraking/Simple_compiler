#pragma once
#include "Lexer/lexer.h"
#include "Symbols/symbols.h"
#include "Inter/stmt.h"

namespace Parser {
	class Parser {
	private:
		Lexer::Lexer& lexer;
		shared_ptr<Lexer::Token> look;
		Symbols::Env top; // ��ǰ�򶥲��ķ��ű�
		int used = 0; // storage used for declarations

	public:
		Parser(Lexer::Lexer& l);

		// ��ȡ��һ���ʷ���Ԫ�� look
		void move();

		void error(std::string s);

		void match(int t);

		void program();

		
	};
}