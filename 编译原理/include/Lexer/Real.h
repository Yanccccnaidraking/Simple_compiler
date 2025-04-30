#pragma once
#include "Token.h"
#include "Tag.h"
namespace Lexer {
	// �� Real ���ڴ�������
	//����float���ͺ�double����
	class Real : public Token
	{
	public:
		const double value;

		Real(float v) : value(v), Token(Tag::REAL) {}

		std::string toString() const { return "real"; }
	};
}

