#pragma once
#include "Token.h"
#include "Tag.h"
namespace Lexer {
	// 类 Bool 用于处理字符
	class Bool : public Token
	{
	public:
		const bool value;

		Bool(bool v) : value(v), Token(Tag::BOOL) {}

		std::string toString() const { std::string str = value ? "true" : "false"; return str; }
	};
}