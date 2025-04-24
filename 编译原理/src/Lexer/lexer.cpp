#include "Lexer/Lexer.h"
#include "Lexer/Num.h"
#include "Lexer/Real.h"
#include "Symbols/symbols.h"
#include <cctype>

namespace Lexer {
	int Lexer::line = 1;

	void Lexer::reserve(const Word& w)
	{
		words.emplace(w.lexeme, w);
	}

	Lexer::Lexer(std::string filepath) : buffer(DoubleBuffer(filepath))
	{
		reserve(Word("if", Tag::IF));
		reserve(Word("else", Tag::ELSE));
		reserve(Word("while", Tag::WHILE));
		reserve(Word("do", Tag::DO));
		reserve(Word("break", Tag::BREAK));
		reserve(Word("switch", Tag::SWITCH));
		reserve(Word("case", Tag::CASE));
		reserve(Word("default", Tag::DEFAULT));
		reserve(Word("return", Tag::RETURN));
		reserve(Word("continue", Tag::CONTINUE));
		reserve(Word::true_);
		reserve(Word::false_);
		// ��ʼ���洢Type �࣬����Type���еĴ洢����ָ�루��������Ĳ����������Դ˴���Ҫ*ȡֵ
		reserve(*Symbols::Type::Int);
		reserve(*Symbols::Type::Long);
		reserve(*Symbols::Type::Bool);
		reserve(*Symbols::Type::Char);
		reserve(*Symbols::Type::Float);
		reserve(*Symbols::Type::Double);
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

	// �ж��Ƿ�Ϊ������
	static bool is_operator(char c) {
		static const std::string operators = "+-*=<>!&|^%~";
		return operators.find(c) != std::string::npos;
	}

	// �ж��Ƿ�Ϊ�ָ���
	static bool is_delimiter(char c) {
		static const std::string delimiters = "(){}[],;:.?";
		return delimiters.find(c) != std::string::npos;
	}

	Lexer::CharType Lexer::getCharType(char c, State currentState) {
		if (c == EOF) return CharType::EOF_CHAR;

		if (currentState == State::END_DELIMITER) {
			return CharType::OTHER_CHAR;
		}

		// ��������
		switch (c) {
		case '\'': return CharType::SINGLE_QOUTE;
		case '"':  return CharType::DOUBLE_QOUTE;
		case '/':  return CharType::FORWARD_SLASH;
		case '\\': return CharType::BACWARD_SLASH;
		case '*':  return CharType::STAR;
		case '\n': return CharType::NEW_LINE;
		}

		// ���ּ��
		if (isdigit(c)) return CharType::DIGIT;

		// ��ĸ���
		if (isalpha(c) || c == '_') return CharType::LETTER;

		// ���������
		if (is_operator(c)) return CharType::OPERATOR;

		// �ָ������
		if (is_delimiter(c)) return CharType::DELIMITER;

		// �ո�����
		if (isspace(c)) return CharType::WHITESPACE;

		return CharType::OTHER_CHAR;
	}



	std::shared_ptr<Token> Lexer::scan() {
		State currentState = State::START;
		std::string lexeme;

		while (true) {
			char c = buffer.cur();
			CharType charType = getCharType(c, currentState);

			if (currentState == State::START && charType == CharType::EOF_CHAR) {
				return nullptr;
			}

			// �����հ��ַ��ͻ��з�
			if (currentState == State::START) {
				if (charType == CharType::WHITESPACE) {
					buffer.next();
					buffer.getToken();
					continue;
				}
				else if (charType == CharType::NEW_LINE) {
					if (c == '\n') line++;  // �����з��������к�
					buffer.next();
					buffer.getToken();
					continue;
				}
				else if (charType == CharType::EOF_CHAR) {
					return nullptr;
				}
			}

			// ����״̬ת�Ʊ����״̬ת��
			auto stateIt = transitionTable.find(currentState);
			if (stateIt == transitionTable.end()) {
				throw std::runtime_error("Invalid state in transition table");
			}
			auto nextStateIt = stateIt->second.find(charType);
			if (nextStateIt == stateIt->second.end()) {
				throw std::runtime_error("Invalid character type for current state");
			}
			State nextState = nextStateIt->second;



			// ��ȡ��һ���ʷ���Ԫ
			if (nextState == State::END) {
				lexeme = buffer.getToken();
				// ���ݵ�ǰ״̬���� Token
				// TODO:��ɸ���״̬��ӦToken�ķ���
				switch (currentState)
				{
				case State::START:
					break;
				case State::IN_ID:
					// ����� words ���У���Ϊ�ؼ���
					if (words.find(lexeme) != words.end())
						return make_shared<Word>(words.at(lexeme));
					else
						return make_shared<Word>(Word(lexeme, Tag::ID));
					break;
				case State::IN_STRING:
					break;
				case State::END_STRING:
					break;
				case State::START_CHAR:
					break;
				case State::IN_CHAR:
					break;
				case State::END_CHAR:
					break;
				case State::START_COMMENT:
					break;
				case State::IN_SINGLE_COMMENT:
					break;
				case State::IN_MUTI_COMMENT:
					break;
				case State::END_MUTI_COMMENT1:
					break;
				case State::END_MUTI_COMMENT2:
					break;
				case State::END_SINGLE_COMMENT:
					break;
				case State::IN_OP: // ���������
					return make_shared<Token>(Token(static_cast<int>(lexeme[0])));
					break;
				case State::END_OP: // ��������� TODO:�޸�Tagֵ
					return make_shared<Word>(Word(lexeme, Tag::TEMP));
					break;
				case State::END_DELIMITER: // ���
					return make_shared<Token>(Token(static_cast<int>(lexeme[0])));
					break;
				case State::END:
					break;
				default:
					break;
				}
				return std::make_shared<Word>(Word::ne);
			}
			else {
				buffer.next();
			}

			currentState = nextState;
		}
	}

	//std::shared_ptr<Token> Lexer::scan()
	//{
	//	// �����Թ����пհ��ַ�
	//	for (;; readch()) {
	//		if (peek == ' ' || peek == '\t') continue;
	//		else if (peek == '\n') line += 1;
	//		else if (peek == EOF) return nullptr;
	//		else break;
	//	}
	//	// ����ʶ�𸴺ϴʷ���Ԫ
	//	switch (peek)
	//	{
	//	case '&':
	//		if (readch('&')) return std::make_shared<Word>(Word::and_); else return std::make_shared<Token>('&');
	//	case '|':
	//		if (readch('|')) return std::make_shared<Word>(Word::or_); else return std::make_shared<Token>('|');
	//	case '=':
	//		if (readch('=')) return std::make_shared<Word>(Word::eq); else return std::make_shared<Token>('=');
	//	case '!':
	//		if (readch('=')) return std::make_shared<Word>(Word::ne); else return std::make_shared<Token>('!');
	//	case '<':
	//		if (readch('=')) return std::make_shared<Word>(Word::le); else return std::make_shared<Token>('<');
	//	case '>':
	//		if (readch('=')) return std::make_shared<Word>(Word::ge); else return std::make_shared<Token>('>');
	//	}
	//	// ����ʶ������
	//	if (std::isdigit(peek)) {
	//		int v = 0;
	//		do {
	//			v = 10 * v + digit(peek, 10);
	//			readch();
	//		} while (std::isdigit(peek));
	//		if (peek != '.') return std::make_shared<Num>(v);
	//		float x = v;
	//		float d = 10;
	//		for (;;) {
	//			readch();
	//			if (!std::isdigit(peek)) break;
	//			x = x + digit(peek, 10) / d; d *= 10;
	//		}
	//		return std::make_shared<Real>(x);
	//	}
	//	// ���Զ����ַ���
	//	if (std::isalpha(peek)) {
	//		std::string s;
	//		do {
	//			s += peek;
	//			readch();
	//		} while (std::isalnum(peek));
	//		try {
	//			Word w = words.at(s);
	//			return std::make_shared<Word>(w);
	//		}
	//		catch (const std::out_of_range& e) {
	//			Word w2(s, Tag::ID);
	//			return std::make_shared<Word>(w2);
	//		}
	//	}
	//	// ����ע��
	//	if (peek == '/') {
	//		// TODO
	//	}
	//	Token tok = Token(peek);
	//	peek = ' ';
	//	return std::make_shared<Token>(tok);
	//}
	//


	int Lexer::digit(char c, int radix)
	{
		if (radix == 10 && std::isdigit(c)) {
			return c - '0';
		}
		return -1;
	}
}