#pragma once
#include "expr.h"
#include "Lexer/lexer.h"
#include "Symbols/symbols.h"
namespace Inter {
	class Id :public Expr
	{
	public:
		int offset;
		Id();
		Id(Lexer::Word id, Symbols::Type p, int b);
	};
}