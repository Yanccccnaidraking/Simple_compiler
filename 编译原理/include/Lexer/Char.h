#pragma once
#include "Token.h"
#include "Tag.h"
namespace Lexer {
	// 类 Char 用于处理字符
	class Char : public Token
	{
	public:
		const char value;

		Char(char v) : value(v), Token(Tag::CHAR) {}

		std::string toString() const { return ""+value; }
	};
}