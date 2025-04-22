#include "Lexer.h"
#include "Num.h"
#include "Real.h"
#include "symbols.h"

namespace Lexer {
	int Lexer::line = 1;

	void Lexer::reserve(const Word& w)
	{
		words.emplace(w.lexeme, w);
	}

	Lexer::Lexer()
	{
		reserve(Word("if", Tag::IF));
		reserve(Word("else", Tag::ELSE));
		reserve(Word("while", Tag::WHILE));
		reserve(Word("do", Tag::DO));
		reserve(Word("break", Tag::BREAK));
		reserve(Word("switch", Tag::SWITCH));
		reserve(Word("case", Tag::CASE));
		reserve(Word("default", Tag::DEFAULT));
		reserve(Word::true_);
		reserve(Word::false_);
		// 初始化存储Type 类，由于Type类中的存储采用指针（方便后续的操作），所以此处需要*取值
		reserve(*Symbols::Type::Int);
		reserve(*Symbols::Type::Bool);
		reserve(*Symbols::Type::Char);
		reserve(*Symbols::Type::Float);
	}


	void Lexer::readch()
	{
		peek = std::cin.get();
	}

	bool Lexer::readch(char c)
	{
		readch();
		if (peek != c) return false;
		peek = ' ';
		return true;
	}

	std::shared_ptr<Token> Lexer::scan()
	{
		// 首先略过所有空白字符
		for (;; readch()) {
			if (peek == ' ' || peek == '\t') continue;
			else if (peek == '\n') line += 1;
			else if (peek == EOF) return nullptr;
			else break;
		}
		// 尝试识别复合词法单元
		switch (peek)
		{
		case '&':
			if (readch('&')) return std::make_shared<Word>(Word::and_); else return std::make_shared<Token>('&');
		case '|':
			if (readch('|')) return std::make_shared<Word>(Word::or_); else return std::make_shared<Token>('|');
		case '=':
			if (readch('=')) return std::make_shared<Word>(Word::eq); else return std::make_shared<Token>('=');
		case '!':
			if (readch('=')) return std::make_shared<Word>(Word::ne); else return std::make_shared<Token>('!');
		case '<':
			if (readch('=')) return std::make_shared<Word>(Word::le); else return std::make_shared<Token>('<');
		case '>':
			if (readch('=')) return std::make_shared<Word>(Word::ge); else return std::make_shared<Token>('>');
		}
		// 尝试识别数字
		if (std::isdigit(peek)) {
			int v = 0;
			do {
				v = 10 * v + digit(peek, 10);
				readch();
			} while (std::isdigit(peek));
			if (peek != '.') return std::make_shared<Num>(v);
			float x = v;
			float d = 10;
			for (;;) {
				readch();
				if (!std::isdigit(peek)) break;
				x = x + digit(peek, 10) / d; d *= 10;
			}
			return std::make_shared<Real>(x);
		}
		// 尝试读入字符串
		if (std::isalpha(peek)) {
			std::string s;
			do {
				s += peek;
				readch();
			} while (std::isalnum(peek));
			try {
				Word w = words.at(s);
				return std::make_shared<Word>(w);
			}
			catch (const std::out_of_range& e) {
				Word w2(s, Tag::ID);
				return std::make_shared<Word>(w2);
			}
		}
		// 处理注释
		if (peek == '/') {
			// TODO
		}
		Token tok = Token(peek);
		peek = ' ';
		return std::make_shared<Token>(tok);
	}
	int Lexer::digit(char c, int radix)
	{
		if (radix == 10 && std::isdigit(c)) {
			return c - '0';
		}
		return -1;
	}
}