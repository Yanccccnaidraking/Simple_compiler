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
		// ��ʼ���洢Type �࣬����Type���еĴ洢����ָ�루��������Ĳ����������Դ˴���Ҫ*ȡֵ
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
		// �����Թ����пհ��ַ�
		for (;; readch()) {
			if (peek == ' ' || peek == '\t') continue;
			else if (peek == '\n') line += 1;
			else if (peek == EOF) return nullptr;
			else break;
		}
		// ����ʶ�𸴺ϴʷ���Ԫ
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
		// ����ʶ������
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
		// ���Զ����ַ���
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
		// ����ע��
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