#pragma once
#include "expr.h"
#include "Lexer/Word.h"
#include "Lexer/lexer.h"
#include "Symbols/symbols.h"

namespace Lexer {
	class Word;
}

namespace Symbols {
	class Type;
}

namespace Inter {
	class Id :public Expr
	{
	public:
		int offset;

		Id(std::shared_ptr<Lexer::Word> id, Symbols::Type p, int b) : Expr(id, &p), offset(b) {}
	};
}