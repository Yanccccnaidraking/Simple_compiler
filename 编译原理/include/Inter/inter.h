#pragma once
#include "Lexer/lexer.h"
#include "Lexer/Word.h"
#include "Symbols/symbols.h"

namespace Symbols
{
	class Type;
}

namespace Inter
{
	class Node
	{
	public:
		int lexline = 0;
		Node();
	};

	class Expr :public Node
	{
	public:
		Expr() :Node(){}
	};

	class Id :public Expr
	{
	public:
		int offset;
		Id();
		Id(Lexer::Word id, Symbols::Type p, int b);
	};
}