#pragma once
#include "Token.h"
#include "Tag.h"
namespace Lexer {
	// 类 Char 用于处理字符
	class Char : public Token
	{
	public:
		const char value;
		const std::string string_value;
		Char(std::string v) :  string_value(v), value(transformToChar()), Token(Tag::CHAR) {}

		std::string toString() const { return string_value; }

		// TODO:完成转换
        inline char transformToChar() {
			return 'x';
        }
	};
}