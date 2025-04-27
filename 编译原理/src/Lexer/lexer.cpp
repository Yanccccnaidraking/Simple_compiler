#include "Lexer/Lexer.h"
#include "Lexer/Num.h"
#include "Lexer/Real.h"
#include "Lexer/Bool.h"
#include "Lexer/Char.h"
#include "Lexer/String.h"
#include "Symbols/symbols.h"
//#include <chrono>
#include <cctype>
#include <map>

namespace Lexer {
	int Lexer::line = 1;

	void Lexer::reserve(const Word& w)
	{
		words.emplace(w.lexeme, w);
	}

	void Lexer::transform(int state, char start, char end, CharType type)
	{
		for (char i = start; i <= end; i++)
		{
			charTypeTable[(State)state][i] = type;
		}
	}

	void Lexer::transform(int state, char c, CharType type)
	{
		charTypeTable[(State)state][c] = type;
	}

	void Lexer::transform(int state, std::string str, CharType type)
	{
		for (auto c : str)
		{
			charTypeTable[(State)state][c] = type;
		}
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
		//loadTableFromFile("CharTypeTable.txt");
		generateAndSaveMap();
	}
	/// <summary>
	/// �����ַ����ͱ�
	/// </summary>
	/// <param name="filename"></param>
	void Lexer::loadTableFromFile(const std::string& filename) {
		// ��¼��ʼʱ��
		//auto start = std::chrono::high_resolution_clock::now();
		std::ifstream in(filename);
		int stateInt, charInt, typeInt;
		while (in >> stateInt >> charInt >> typeInt) {
			charTypeTable[static_cast<State>(stateInt)][static_cast<char>(charInt)] = static_cast<CharType>(typeInt);
		}
		in.close();
		// ��¼����ʱ��
		//auto end = std::chrono::high_resolution_clock::now();
		// �����ʱ����λ�����룩
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		//std::cout << "��ʱ��" << duration.count() << " ����" << std::endl;
	}

	std::shared_ptr<Word> Lexer::getOperationToken(const std::string& op)
	{
		static std::map<std::string, Tag> opToTag = {
			{"++", Tag::PLUS_PLUS},
			{"--", Tag::MINUS_MINUS},
			{"+=", Tag::PLUS_EQ},
			{"-=", Tag::MINUS_EQ},
			{"*=", Tag::MULT_EQ},
			{"/=", Tag::DIV_EQ},
			{"%=", Tag::MOD_EQ},
			{"&=", Tag::AND_EQ},
			{"|=", Tag::OR_EQ},
			{"^=", Tag::XOR_EQ},
			{"<<", Tag::LEFT_SHIFT},
			{">>", Tag::RIGHT_SHIFT},
			{"->", Tag::ARROW},
			{">=", Tag::GE},
			{"==", Tag::EQ},
			{"<=", Tag::LE},
			{"&&", Tag::AND},
			{"||", Tag::OR},
			{"!=", Tag::NE},
		};

		auto it = opToTag.find(op);
		if (it != opToTag.end()) {
			return std::make_shared<Word>(op, it->second);
		}
		throw std::runtime_error("line " + to_string(line) + ": Invalid opearation.");
		return nullptr;
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

	// TODO: ����Ϊ��ÿ��currentState�����жϣ������ͻ
	Lexer::CharType Lexer::getCharType(char c, State currentState) {
		if (c == EOF) return CharType::EOF_CHAR;
		//���������ڼ����ض�Ӧ�����ͣ���������ھͷ���OTHER_CHAR
		auto stateIt = charTypeTable.find(currentState);
		if (stateIt != charTypeTable.end()) {
			auto charIt = stateIt->second.find(c);
			if (charIt != stateIt->second.end()) {
				return charIt->second;
			}
		}
		return CharType::OTHER_CHAR;

		//if (currentState == State::END_DELIMITER) {
		//	return CharType::OTHER_CHAR;
		//}

		//if (currentState == State::START) {
		//	if (c > '0' && c <= '9')
		//		return CharType::DIGIT_ONE;
		//	else if (c == '0')
		//		return CharType::ZERO;
		//}

		//if (((int)currentState) / 100 == 3) {//�ж��Ƿ������ַ�֧�У���3��ͷ��״̬��
		//	switch (tolower(c))
		//	{
		//	case 'l':
		//		return CharType::LONG_SIGN;
		//		break;
		//	case 'e':
		//		return CharType::SCI_SIGN;
		//		break;
		//	case 'f':
		//		return CharType::FLOAT_SIGN;
		//		break;
		//	case 'x':
		//		return CharType::HEX_SIGN;
		//		break;
		//	case 'b':
		//		return CharType::BIN_SIGN;
		//		break;
		//	default:
		//		break;
		//	}
		//}

		//if (currentState == State::IN_OCT_NUM)
		//{
		//	if (c >= '0' && c < '8')
		//		return CharType::DIGIT;//�ض���Ϸ�����
		//}

		//// ��������
		//switch (c) {
		//case '\'': return CharType::SINGLE_QOUTE;
		//case '"':  return CharType::DOUBLE_QOUTE;
		//case '/':  return CharType::FORWARD_SLASH;
		//case '\\': return CharType::BACWARD_SLASH;
		//case '*':  return CharType::STAR;
		//case '\n': return CharType::NEW_LINE;
		//}

		//// ���������
		//if (is_operator(c)) return CharType::OPERATOR;

		//// �ڲ���������� OP ״̬
		//if (currentState == State::IN_OP || currentState == State::END_OP) {
		//	return CharType::OTHER_CHAR;
		//}

		//// ���ּ��
		//if (isdigit(c)) return CharType::DIGIT;


		//// ��ĸ���
		//if (isalpha(c) || c == '_') return CharType::LETTER;

		//// �ָ������
		//if (is_delimiter(c)) return CharType::DELIMITER;

		//// �ո�����
		//if (isspace(c)) return CharType::WHITESPACE;

		//return CharType::OTHER_CHAR;
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

			try {
				// ����״̬ת�Ʊ����״̬ת��
				auto stateIt = transitionTable.find(currentState);
				if (stateIt == transitionTable.end()) {
					throw std::runtime_error("line " + to_string(line) + ": Invalid state in transition table");
				}
				auto nextStateIt = stateIt->second.find(charType);
				if (nextStateIt == stateIt->second.end()) {
					throw std::runtime_error("line " + to_string(line) + ": Invalid character type for current state");
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
					return make_shared<String>(String(lexeme));
					break;
				case State::START_CHAR:
					break;
				case State::IN_CHAR:
					break;
				case State::END_CHAR:
					break;
				case State::START_COMMENT:
					return make_shared<Token>(Token(static_cast<int>(lexeme[0])));
					break;
				case State::IN_SINGLE_COMMENT:
					currentState = State::START;
					continue;
					break;
				case State::IN_MUTI_COMMENT:
					break;
				case State::END_MUTI_COMMENT1:
					currentState = State::START;
					buffer.next();
					buffer.getToken();
					continue;
					break;
				case State::END_MUTI_COMMENT2:
					break;
				case State::END_SINGLE_COMMENT:
					break;
				case State::IN_OP: // ���������
					return make_shared<Token>(Token(static_cast<int>(lexeme[0])));
					break;
				case State::END_OP: // ��������� TODO:�޸�Tagֵ
					return getOperationToken(lexeme);
					break;
				case State::END_DELIMITER: // ���
					return make_shared<Token>(Token(static_cast<int>(lexeme[0])));
					break;
				case State::IN_NUM:
				{
					int value = std::stoi(lexeme);
					return make_shared<Num>(Num(value));
					break;
				}
				case State::END_NUM_LONG:
				{
					long value_long = std::stol(lexeme);
					return make_shared<Num>(Num(value_long));
					break;
				}
				case State::END_SCI_NUM://�������״̬��������һ�£����ظ���д
				case State::END_REAL:
				{
					double value_double = std::stod(lexeme);
					return make_shared<Real>(Real(value_double));
					break;
				}
				case State::IN_OCT_NUM:
				{
					int value_oct = std::stoi(lexeme, nullptr, 8);
					return make_shared<Num>(Num(value_oct));
					break;
				}
				case State::END_SCI_NUM_F:
				{
					float value_float = std::stof(lexeme);
					return make_shared<Real>(Real(value_float));
				}
				case State::IN_HEX_NUM:
				{
					int value_hex = std::stoi(lexeme, nullptr, 16);
					return make_shared<Num>(Num(value_hex));
				}
				case State::IN_BIN_NUM:
				{
					if (lexeme.rfind("0b", 0) == 0) {
						lexeme = lexeme.substr(2);
					}
					int value_bin = std::stoi(lexeme, nullptr, 2);
					return make_shared<Num>(Num(value_bin));
				}
				case State::IN_NORMAL_CHAR: // 202
					break;
				case State::IN_ESCAPE_STATE: // 203
					break;
				case State::IN_PARSE_OBT_1://204
					break;
				case State::IN_PARSE_OBT_2://205
					break;
				case State::IN_PARSE_OBT_3://206
					break;
				case State::IN_PARSE_HEX_1://207
					break;
				case State::IN_PARSE_HEX_n://208
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
			catch (std::runtime_error err) {
				error_info += err.what();
				error_info += '\n';
				currentState = State::START;
				buffer.next();
				buffer.getToken();
			}
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