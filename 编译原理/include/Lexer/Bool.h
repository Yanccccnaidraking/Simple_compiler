#pragma once
#include "Token.h"
#include "Tag.h"
namespace Lexer {
	// �� Bool ���ڴ����ַ�
	class Bool : public Token
	{
	public:
		const bool value;

		Bool(bool v) : value(v), Token(Tag::BOOL) {}

		std::string toString() const { std::string str = value ? "true" : "false"; return str; }
	};
}