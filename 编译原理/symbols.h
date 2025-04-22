#pragma once
#include<unordered_map>
#include "lexer.h"
#include "inter.h"
#include "Token.h"
using namespace std;

namespace Symbols
{
	class Env {
	private:
		unordered_map<Lexer::Token,  Inter::Id> table;
	protected:
		Env* prev;
	public:
		Env(Env* n);
		void put(Lexer::Token w, Inter::Id i);
		Inter::Id* get(Lexer::Token w);
	};

	class Type :Lexer::Word
	{
	public:
		int width = 0;
		Type();
		Type(const string& s, int tag, int w);
		Type(const string& s, Lexer::Tag tag, int w);
		const static Type* Int, * Short, * Float, * Char, * Bool, * Double, * Long;
		static bool numeric(const Type* p);
		static const Type* max(const Type* p1, const Type* p2);
		static int typePriority(const Type* type);
	};
}
