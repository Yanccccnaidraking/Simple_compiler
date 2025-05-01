#pragma once
#include "expr.h"
#include "temp.h"
namespace Inter {
	class Op : public Expr {
	public:
		Op(Lexer::Token tok, Symbols::Type* p) : Expr(tok, p) {}

		shared_ptr<Expr> reduce() {
			Expr& x = gen();
			std::shared_ptr<Temp> t = std::make_shared<Temp>(type);
			std::cout << t->toString() << " = " << x.toString() << std::endl;
			return t;
		}
	};
}
