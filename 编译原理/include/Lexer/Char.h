#pragma once
#include "Token.h"
#include "Tag.h"
namespace Lexer {
	// �� Char ���ڴ����ַ�
	class Char : public Token
	{
	public:
		const char value;

		Char(char v) : value(v), Token(Tag::CHAR) {}

		std::string toString() const { return ""+value; }
	};
}