#pragma once
#include "Token.h"
#include "Tag.h"
namespace Lexer {
	// �� Char ���ڴ����ַ�
	class Char : public Token
	{
	public:
		const char value;
		const std::string string_value;
		Char(std::string v) :  string_value(v), value(transformToChar()), Token(Tag::CHAR) {}

		std::string toString() const { return string_value; }

		// TODO:���ת��
        inline char transformToChar() {
			return 'x';
        }
	};
}