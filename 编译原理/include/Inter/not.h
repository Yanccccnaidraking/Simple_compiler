#pragma once
#include"logical.h"
#include"Lexer/Token.h"

#include"string"

namespace Inter
{


	class Not :public Logical
	{

	public: 
		Not(std::shared_ptr<Lexer::Token> tok, std::shared_ptr<Expr> x2) : Logical(tok, x2, x2) {}
		void jumping(int t, int f) { expr2->jumping(f, t); }
		string toString() const { return op->toString() + " " + expr2->toString(); }
	};

}