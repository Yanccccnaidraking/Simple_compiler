#pragma once
#include "node.h"
#include "Symbols/symbols.h"
#include "Lexer/Token.h" 
#include "Lexer/Word.h"  
#include <memory>

namespace Inter {
	class Expr :public Node
	{
	public:
		std::shared_ptr<Lexer::Token> op;
		std::shared_ptr<Symbols::Type> type;

		Expr(std::shared_ptr<Lexer::Token> tok, std::shared_ptr<Symbols::Type>  p) : op(tok), type(p) {}

		Expr& gen() { return *this; }
		Expr& reduce() { return *this; }

		void jumping(int t, int f) {
			emitjumps(toString(), t, f);
		}

		void emitjumps(std::string test, int t, int f) {
			if (t != 0 && f != 0) {
				emit("if " + test + " goto L" + std::to_string(t));
				emit("goto L" + std::to_string(f));
			}
			else if (t != 0) {
				emit("if " + test + " goto L" + std::to_string(t));
			}
			else if (f != 0) {
				emit("iffalse " + test + " goto L" + std::to_string(f));
			}
		} 

		std::string toString() const { return op->toString(); }
	};
}