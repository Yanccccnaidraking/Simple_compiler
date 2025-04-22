#pragma once
#include <string>
#include <iostream>
#include "Tag.h"
namespace Lexer {
	class Token
	{
	public:
		const int tag;
		Token() :tag(-1) {}
		Token(int t) : tag(t) {}
		Token(Tag t) : tag(int(t)) {}
		// Token(const Token& other): tag(other.tag) {}

		virtual std::string toString() const {
			return std::string(1, static_cast<char>(tag));
		}
	};
}
