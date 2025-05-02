#pragma once
#include<unordered_map>
#include "Lexer/lexer.h"
#include "Lexer/Word.h"
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
		static Type* Int, * Short, * Float, * Char, * Bool, * Double, * Long;
		static Type* max(const Type* p1, const Type* p2);
		static int typePriority(const Type* type);
		Type& operator= (Type& t) { this->width = t.width;  return *this; }

		bool operator==(Type& other) const {
			return this->tag == other.tag &&
				this->lexeme == other.lexeme &&
				this->width == other.width;
		}

		bool operator!=(Type& other) const {
			return !(*this == other);
		}
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
