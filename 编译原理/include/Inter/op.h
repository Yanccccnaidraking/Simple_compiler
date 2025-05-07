#pragma once
#include "expr.h"
#include "temp.h"
#include <memory>
namespace Inter {
	class Op : public Expr {
	public:
		Op(std::shared_ptr<Lexer::Token> tok, std::shared_ptr<Symbols::Type> p) : Expr(tok, p) {}


		std::shared_ptr<Expr> reduce() {
			std::shared_ptr<Expr> x = gen();
			Expr& x_ref = *x;
			std::shared_ptr<Temp> t = std::make_shared<Temp>(type);
			std::cout << t->toString() << " = " << x_ref.toString() << std::endl;
			return t;
		}
	};
}
