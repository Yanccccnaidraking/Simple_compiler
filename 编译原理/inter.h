#pragma once
#include "lexer.h"
#include "Word.h"
#include "symbols.h"

namespace Inter
{
	class Node
	{
		int lexline = 0;
		Node() { lexline = Lexer::Lexer::line; }
	};

	class Expr :Node
	{

	};

	class Id :Expr
	{
	public:
		int offset;
		Id();
		Id(Lexer::Word id, Symbols::Type p, int b);
	};
}