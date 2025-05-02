#pragma once
#include "expr.h"
#include "temp.h"
#include <memory>
namespace Inter {
	class Op : public Expr {
	public:
		Op(Lexer::Token* tok, Symbols::Type* p) : Expr(tok, p) {}

		std::shared_ptr<Expr> reduce() {
			Expr& x = gen();
			std::shared_ptr<Temp> t = std::make_shared<Temp>(type);
			std::cout << t->toString() << " = " << x.toString() << std::endl;
			return t;
		}
	};
}
