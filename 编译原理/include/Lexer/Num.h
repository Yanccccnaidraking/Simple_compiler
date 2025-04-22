#pragma once
#include "Token.h"
#include "Tag.h"
namespace Lexer {
	class Num : public Token
	{
	public:
		const int value;

		Num(int v) : value(v), Token(Tag::NUM) {}

		std::string toString() const { return std::to_string(value); }
	};
}
