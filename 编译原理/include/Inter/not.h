#pragma once
#include"logical.h"
#include"Lexer/Token.h"

#include"string"

namespace Inter
{


	class Not :public Logical
	{

	public: 
		Not(Lexer::Token tok, Expr x2);
		void jumping(int t, int f);
		string toString();
	};

}