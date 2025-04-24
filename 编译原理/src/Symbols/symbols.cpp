#include "Symbols/symbols.h"
#include "Lexer/Token.h"
#include<math.h>

namespace Symbols
{
	Env::Env(Env* e)
	{
		prev = e;
	}

	/// <summary>
	/// 将字符串词法单元映射成类Id对象
	/// </summary>
	/// <param name="w">词法单元</param>
	/// <param name="i">对应的Id</param>
	void Env::put(Lexer::Token w, shared_ptr<Inter::Id> i)
	{
		this->table[w] = i;
	}
	/// <summary>
	/// 依据词法单元Token获取对应的Id
	/// </summary>
	/// <param name="w">词法单元</param>
	/// <returns>Id类</returns>
	shared_ptr<Inter::Id> Env::get(Lexer::Token w)
	{
		for (Env* e = this; e != nullptr; e = e->prev)
		{
			auto found = e->table.find(w);
			if (found != table.end())
			{
				return make_shared<Inter::Id>(*(found->second));
			}
		}
		return nullptr;
	}

	Type::Type() :Word()
	{
		this->width = 0;
	}

	Type::Type(const string& s, int tag, int w) :Word(s, tag)
	{
		this->width = w;
	}

	Type::Type(const string& s, Lexer::Tag tag, int w) :Word(s, tag)
	{
		this->width = w;
	}


	/// <summary>
	/// 判断变量优先级和变量是否合法
	/// </summary>
	/// <param name="type">变量类型</param>
	/// <returns>变量的优先级</returns>
	int Type::typePriority(const Type* type) {
		if (type == Type::Bool)        return 0;
		else if (type == Type::Char)   return 1;
		else if (type == Type::Short)  return 2;
		else if (type == Type::Int)    return 3;
		else if (type == Type::Long)   return 4;
		else if (type == Type::Float)  return 5;
		else if (type == Type::Double) return 6;
		else return -1;
	}

	/// <summary>
	/// 两数运算时的类型转换
	/// </summary>
	/// <param name="p1">类型1</param>
	/// <param name="p2">类型2</param>
	/// <returns></returns>
	const Type* Type::max(const Type* p1, const Type* p2)
	{
		int pr1 = typePriority(p1);
		int pr2 = typePriority(p2);
		if (pr1 == -1 || pr2 == -1)
		{
			return nullptr;
		}
		int maxPr = std::max(pr1, pr2);
		switch (maxPr)
		{
		case 0:
			return Type::Bool;
		case 1:
			return Type::Char;
		case 2:
			return Type::Short;
		case 3:
			return Type::Int;
		case 4:
			return Type::Long;
		case 5:
			return Type::Float;
		case 6:
			return Type::Double;
		default:
			return nullptr;
		}
	}

	//定义每一个类型的字节数，Tag类型以及对应代码中的写法
	const Type* Type::Bool = new Type("bool", Lexer::Tag::BASIC, 1);
	const Type* Type::Char = new Type("char", Lexer::Tag::BASIC, 1);
	const Type* Type::Short = new Type("short", Lexer::Tag::BASIC, 2);
	const Type* Type::Int = new Type("int", Lexer::Tag::BASIC, 4);
	const Type* Type::Long = new Type("long", Lexer::Tag::BASIC, 4);
	const Type* Type::Float = new Type("float", Lexer::Tag::BASIC, 4);
	const Type* Type::Double = new Type("double", Lexer::Tag::BASIC, 8);
}

