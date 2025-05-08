#pragma once
#include<unordered_map>
#include "Lexer/lexer.h"
#include "Lexer/Word.h"
using namespace std;


// 前向声明 Inter::Id
namespace Inter {
	class Id;
}

namespace Symbols
{
	class Env {
	private:
		unordered_map<std::string, shared_ptr<Inter::Id>> table;
	protected:
		std::shared_ptr<Env> prev;
	public:
		Env(std::shared_ptr<Env> n);
		void put(std::string w, shared_ptr<Inter::Id> i);
		shared_ptr<Inter::Id> get(std::string w);
		/// 退出作用域：返回上层作用域
		static std::shared_ptr<Env> exit(std::shared_ptr<Env> current) {
			return current ? current->prev : nullptr;
		}
		unordered_map<std::string, shared_ptr<Inter::Id>> getTable() {
			return table;
		}
	};

	class Type :public Lexer::Word
	{
	public:
		int width = 0;
		Type();
		Type(const string& s, int tag, int w);
		Type(const string& s, Lexer::Tag tag, int w);
		static std::shared_ptr<Type> Int,  Short,  Float,  Char,  Bool,  Double,  Long;
		static std::shared_ptr<Type> max(const std::shared_ptr<Type> p1, const std::shared_ptr<Type> p2);
		static int typePriority(const std::shared_ptr<Type> type);
		static bool numeric(const std::shared_ptr<Type> type);
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
		std::shared_ptr<Type> of;
		int size = 1;
		Array(int sz, std::shared_ptr<Type> p):Type("[]", Lexer::Tag::INDEX, sz* (p->width)) {
			size = sz;
			of = p;
		}
		std::string toString() {
			return "[" + std::to_string(size) + "]" + of->toString();
		}
	};
}
