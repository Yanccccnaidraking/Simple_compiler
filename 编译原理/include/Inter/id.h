#pragma once
#include "expr.h"
#include "Lexer/lexer.h"
#include "Symbols/symbols.h"
namespace Inter {
	class Id :public Expr
	{
	public:
		int offset;
	};
}