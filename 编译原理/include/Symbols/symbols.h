#pragma once
#include<unordered_map>
#include "Lexer/lexer.h"
#include "Inter/inter.h"
#include "Lexer/Token.h"
using namespace std;


// Ç°ÏòÉùÃ÷ Inter::Id
namespace Inter {
	class Id;
}

namespace Symbols
{
	class Env {
	private:
		unordered_map<Lexer::Token,  shared_ptr<Inter::Id>> table;
	protected:
		Env* prev;
	public:
		Env(Env* n);
		void put(Lexer::Token w, shared_ptr<Inter::Id> i);
		shared_ptr<Inter::Id> get(Lexer::Token w);
	};

	class Type :public Lexer::Word
	{
	public:
		int width = 0;
		Type();
		Type(const string& s, int tag, int w);
		Type(const string& s, Lexer::Tag tag, int w);
		const static Type* Int, * Short, * Float, * Char, * Bool, * Double, * Long;
		static const Type* max(const Type* p1, const Type* p2);
		static int typePriority(const Type* type);
		Type& operator= (const Type& t) { this->width = t.width;  return *this; }
	};

	class Array : public Type {
	public:
		Type of;
		int size = 1;
		Array(int sz, Type p):Type("[]",Lexer::Tag::INDEX,sz*p.width) {
			size = sz;
			of = p;
		}
		std::string toString() {
			return "[" + std::to_string(size) + "]" + of.toString();
		}
	};
}
