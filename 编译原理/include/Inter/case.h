#pragma once
#include "Inter/stmt.h"
#include "Inter/expr.h"
#include "Lexer/Tag.h"
namespace Inter {
	class Case :public Stmt
	{
		std::shared_ptr<Inter::Expr> expr;
		std::shared_ptr<Inter::Stmt> stmt;
	public :
		Case(std::shared_ptr<Inter::Expr> e, std::shared_ptr<Inter::Stmt>s) : expr(e), stmt(s)
		{
			if (e&&e->type != Symbols::Type::Int)
			{
				expr->error("switch语句判断必须是整数类型");
			}
			Enclosing = stmt->Enclosing;
		}
		void gen(int b, int a)
		{
			stmt->gen(b, a);
		}
		void emitCase(std::string test, std::string value,int label)
		{
			emit("case " + test + " " + value + " L" + std::to_string(label));
		}
		std::shared_ptr<Inter::Expr> reduce()
		{
			return expr->reduce();
		}
	};
}