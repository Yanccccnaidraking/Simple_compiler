#include "Inter/inter.h"
#include "Lexer/Word.h"
#include "Symbols/symbols.h"

namespace Inter
{
	Node::Node()
	{
		lexline = Lexer::Lexer::line;
	}

	Id::Id()
	{
		offset = 0;
	}

	Id::Id(Lexer::Word id, Symbols::Type p,int b)
	{
		offset = b;
	}
	
}