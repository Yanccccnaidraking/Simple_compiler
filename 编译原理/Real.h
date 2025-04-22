#pragma once
#include "Token.h"
#include "Tag.h"
namespace Lexer {
	// �� Real ���ڴ�������
	class Real : public Token
	{
	public:
		const float value;

		Real(float v) : value(v), Token(Tag::REAL) {}

		std::string toString() const { return std::to_string(value); }
	};
}

