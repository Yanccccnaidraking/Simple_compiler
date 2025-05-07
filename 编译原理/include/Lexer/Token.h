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
		virtual ~Token() = default;
		// Token(const Token& other): tag(other.tag) {}

		bool operator==(const Token& other) const {
			return tag == other.tag;
		}

		virtual std::string toString() const {
			return std::string(1, static_cast<char>(tag));
		}
	};
}

namespace std {
	template <>
	struct hash<Lexer::Token> {
		size_t operator()(const Lexer::Token& token) const {
			return hash<int>()(token.tag);
		}
	};
}
