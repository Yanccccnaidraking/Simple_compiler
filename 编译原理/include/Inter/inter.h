#pragma once
#include "Lexer/lexer.h"
#include "Lexer/Word.h"
#include "Symbols/symbols.h"

namespace Inter
{
	class Node
	{
		int lexline = 0;
		Node() { lexline = Lexer::Lexer::line; }
	};

	class Expr :public Node
	{

	};

	class Id :public Expr
	{
	public:
		int offset;
		Id();
		Id(Lexer::Word id, Symbols::Type p, int b);
	};
}