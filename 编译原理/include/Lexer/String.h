#pragma once
#include "Token.h"
#include "Tag.h"
namespace Lexer {
	// 类 String 用于处理字符串
	class String : public Token
	{
	public:
		const std::string value;

		String(std::string v) : value(v), Token(Tag::STRING) {}

		std::string toString() const { return value; }
	};
}