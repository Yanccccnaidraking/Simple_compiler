#pragma once
#include "Lexer/lexer.h"
#include "Symbols/symbols.h"

namespace Symbols {
	class Type;
}

namespace Inter {
	class Node
	{
	public:
		int lexline;

		static int labels;

		Node() : lexline(Lexer::Lexer::line) {}

		virtual ~Node() = default;

		void error(std::string s) {
			throw std::runtime_error("line + " + std::to_string(lexline) + ": " + s);
		}

		int newlabel() { return labels++; }

		void emitlabel(int i) { std::cout << "L" + std::to_string(i) + ":"; }

		virtual void emit(std::string s) { std::cout << "\t" + s << std::endl; }
	};
}