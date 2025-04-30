#pragma once
#include "Token.h"
#include "Tag.h"
namespace Lexer {
	// 类 Real 用于处理浮点数
	//包括float类型和double类型
	class Real : public Token
	{
	public:
		const double value;

		Real(float v) : value(v), Token(Tag::REAL) {}

		std::string toString() const { return "real"; }
	};
}

