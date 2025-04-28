#include "Lexer/Lexer.h"
#include "Lexer/Num.h"
#include "Lexer/Real.h"
#include "Lexer/Bool.h"
#include "Lexer/Char.h"
#include "Lexer/String.h"
#include "Symbols/symbols.h"
//#include <chrono>
#include <charconv>
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
		// 初始化存储Type 类，由于Type类中的存储采用指针（方便后续的操作），所以此处需要*取值
		reserve(*Symbols::Type::Int);
		reserve(*Symbols::Type::Long);
		reserve(*Symbols::Type::Bool);
		reserve(*Symbols::Type::Char);
		reserve(*Symbols::Type::Float);
		reserve(*Symbols::Type::Double);
		//loadTableFromFile("CharTypeTable.txt");
		generateAndSaveMap();
	}

	Lexer& Lexer::operator=(Lexer& lexer)
	{
		return lexer;
	}

	/// <summary>
	/// 加载字符类型表
	/// </summary>
	/// <param name="filename"></param>
	void Lexer::loadTableFromFile(const std::string& filename) {
		// 记录开始时间
		//auto start = std::chrono::high_resolution_clock::now();
		std::ifstream in(filename);
		int stateInt, charInt, typeInt;
		while (in >> stateInt >> charInt >> typeInt) {
			charTypeTable[static_cast<State>(stateInt)][static_cast<char>(charInt)] = static_cast<CharType>(typeInt);
		}
		in.close();
		// 记录结束时间
		//auto end = std::chrono::high_resolution_clock::now();
		// 计算耗时（单位：毫秒）
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		//std::cout << "耗时：" << duration.count() << " 毫秒" << std::endl;
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

	// 判断是否为操作符
	static bool is_operator(char c) {
		static const std::string operators = "+-*=<>!&|^%~";
		return operators.find(c) != std::string::npos;
	}

	// 判断是否为分隔符
	static bool is_delimiter(char c) {
		static const std::string delimiters = "(){}[],;:.?";
		return delimiters.find(c) != std::string::npos;
	}

	// TODO: 更改为对每个currentState单独判断，避免冲突
	Lexer::CharType Lexer::getCharType(char c, State currentState) {
		if (c == EOF) return CharType::EOF_CHAR;
		//查表，如果存在即返回对应的类型，如果不存在就返回OTHER_CHAR
		auto stateIt = charTypeTable.find(currentState);
		if (stateIt != charTypeTable.end()) {
			auto charIt = stateIt->second.find(c);
			if (charIt != stateIt->second.end()) {
				return charIt->second;
			}
		}
		return CharType::OTHER_CHAR;
	}

	bool canConvertToInt(const std::string& str)
	{
		int base = 10;
		const char* begin = str.data();
		const char* end = str.data() + str.size();

		// 检查前缀判断进制
		if (str.size() > 2 && str[0] == '0') {
			char second = std::tolower(str[1]);
			if (second == 'x') {
				base = 16;
				begin += 2; // 跳过 0x
			}
			else if (second == 'b') {
				base = 2;
				begin += 2; // 跳过 0b
			}
			else if (std::isdigit(str[1])) {
				base = 8;
				begin += 1; // 0开头，八进制（注意不要跳过第一个数字）
			}
		}

		int value;
		auto result = std::from_chars(begin, end, value, base);
		return result.ec == std::errc();
	}

	bool canConvertToFloat(const std::string& str) {
		const char* begin = str.data();
		const char* end = str.data() + str.size()-1;
		float value;
		auto result = std::from_chars(begin, end, value);
		return result.ec == std::errc() && result.ptr == end;
	}

	bool canConvertToDouble(const std::string& str) {
		const char* begin = str.data();
		const char* end = str.data() + str.size();
		double value;
		auto result = std::from_chars(begin, end, value);
		return result.ec == std::errc() && result.ptr == end;
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

			// 记录注释中的换行
			if (currentState == State::IN_MUTI_COMMENT ||
				currentState == State::END_MUTI_COMMENT1) {
				if (c == '\n') {
					line++;
				}
			}

			// 跳过空白字符和换行符
			if (currentState == State::START) {
				if (charType == CharType::WHITESPACE) {
					buffer.next();
					buffer.getToken();
					continue;
				}
				else if (charType == CharType::NEW_LINE) {
					if (c == '\n') line++;  // 处理换行符，更新行号
					buffer.next();
					buffer.getToken();
					continue;
				}
				else if (charType == CharType::OTHER_CHAR) {
					buffer.next();
					buffer.getToken();
					return std::make_shared<Word>(Word::ne);
				}
				else if (charType == CharType::EOF_CHAR) {
					return nullptr;
				}
			}

			try {
				// 根据状态转移表进行状态转移
				auto stateIt = transitionTable.find(currentState);
				if (stateIt == transitionTable.end()) {
					throw std::runtime_error("line " + to_string(line) + ": Invalid state in transition table");
				}
				auto nextStateIt = stateIt->second.find(charType);
				if (nextStateIt == stateIt->second.end()) {
					throw std::runtime_error("line " + to_string(line) + ": Invalid character type for current state");
				}
				State nextState = nextStateIt->second;



			// 获取到一个词法单元
			if (nextState == State::END) {
				lexeme = buffer.getToken();
				// 根据当前状态返回 Token
				// TODO:完成各个状态对应Token的返回
				switch (currentState)
				{
				case State::START:
					break;
				case State::IN_ID:
					// 如果在 words 表中，则为关键字
					if (words.find(lexeme) != words.end())
						return make_shared<Word>(words.at(lexeme));
					else
						return make_shared<Word>(Word(lexeme, Tag::ID));
					break;
				case State::START_STRING:
					break;
				case State::START_CHAR: //401
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
				case State::IN_OP: // 单个运算符
					return make_shared<Token>(Token(static_cast<int>(lexeme[0])));
					break;
				case State::END_OP: // 复合运算符 TODO:修改Tag值
					return getOperationToken(lexeme);
					break;
				case State::END_DELIMITER: // 界符
					return make_shared<Token>(Token(static_cast<int>(lexeme[0])));
					break;
				case State::IN_NUM:
				{
					if (canConvertToInt(lexeme))
					{
						int value = std::stoi(lexeme);
						return make_shared<Num>(Num(value));
					}
					else
					{
						throw std::runtime_error("line " + to_string(line) + ": Integer constant is too large(整型常数太大)");
					}
					break;
				}
				case State::END_NUM_LONG:
				{
					if (canConvertToInt(lexeme))
					{
						long value_long = std::stol(lexeme);
						return make_shared<Num>(Num(value_long));
					}
					else
					{
						throw std::runtime_error("line " + to_string(line) + ": Integer constant is too large(整型常数太大)");
					}
					break;
				}
				case State::END_SCI_NUM://和下面的状态返回类型一致，不重复编写
				case State::END_REAL:
				{
					if (canConvertToDouble(lexeme))
					{
						double value_double = std::stod(lexeme);
						return make_shared<Real>(Real(value_double));
					}
					else
					{
						throw std::runtime_error("line " + to_string(line) + ": Floating-point constant exceeds allowed range(浮点常数超出范围)");
					}
					break;
				}
				case State::IN_OCT_NUM:
				{
					if (canConvertToInt(lexeme))
					{
						int value_oct = std::stoi(lexeme, nullptr, 8);
						return make_shared<Num>(Num(value_oct));
					}
					else
					{
						throw std::runtime_error("line " + to_string(line) + ": Integer constant is too large(整型常数太大)");
					}
					break;
				}
				case State::END_SCI_NUM_F:
				{
					if (canConvertToFloat(lexeme))
					{
						float value_float = std::stof(lexeme);
						return make_shared<Real>(Real(value_float));
					}
					else
					{
						throw std::runtime_error("line " + to_string(line) + ": Floating-point constant exceeds allowed range(浮点常数超出范围)");
					}
					break;
				}
				case State::END_HEX_NUM:
				{
					if (canConvertToInt(lexeme))
					{
						int value_hex = std::stoi(lexeme, nullptr, 16);
						return make_shared<Num>(Num(value_hex));
					}
					else
					{
						throw std::runtime_error("line " + to_string(line) + ": Integer constant is too large(整型常数太大)");
					}
					break;
				}
				case State::END_BIN_NUM:
				{
					if (canConvertToInt(lexeme))
					{
						if (lexeme.rfind("0b", 0) == 0) {
							lexeme = lexeme.substr(2);
						}
						int value_bin = std::stoi(lexeme, nullptr, 2);
						return make_shared<Num>(Num(value_bin));
					}
					else
					{
						throw std::runtime_error("line " + to_string(line) + ": Integer constant is too large(整型常数太大)");
					}
					break;
				}
				case State::IN_NORMAL_STRING_CHAR: //202
					break;
				case State::IN_ESCAPE_STATE: //203
					break;
				case State::IN_PARSE_OCT_1://204
					break;
				case State::IN_PARSE_OCT_2://205
					break;
				case State::IN_PARSE_OCT_3://206
					break;
				case State::IN_PARSE_HEX_1://207
					break;
				case State::IN_PARSE_HEX_n://208
					break;
				case State::END_STRING://209
					return make_shared<String>(String(lexeme));
					break;
				case State::ERROR_UNCLOSED_STRING://210
					throw std::runtime_error("line " + to_string(line) + ": missing terminating \" character."+"Primitive string:"+ lexeme);
					return make_shared<String>(String(lexeme));
					break;
				case State::IN_NORMAL_CHAR://402
					break;
				case State::IN_ESCAPE_CHAR://403
					break;
				case State::IN_PARSE_OCT_CHAR_1://404
					break;
				case State::IN_PARSE_OCT_CHAR_2://405
					break;
				case State::IN_PARSE_OCT_CHAR_3://406
					break;
				case State::IN_PARSE_HEX_CHAR_1://407
					break;
				case State::IN_PARSE_HEX_CHAR_n://408
					break;
				case State::END_CHAR: //409
					//return make_shared<String>(String(lexeme));
					//std::cout << lexeme << std::endl;
					return make_shared<Char>(Char(lexeme));
					break;
				case State::ERROR_UNCLOSED_CHAR://410
					break;
				case State::ERROR_EMPTY_CHAR://411
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


	int Lexer::digit(char c, int radix)
	{
		if (radix == 10 && std::isdigit(c)) {
			return c - '0';
		}
		return -1;
	}
}