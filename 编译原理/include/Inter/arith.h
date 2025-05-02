#pragma once

#include "Op.h"
#include "expr.h"
#include "Lexer/Token.h" 
#include "Symbols/symbols.h"
#include <sstream>
#include <stdexcept>
#include <memory>

namespace Inter
{
    class Arith : public Op {
    protected:
        std::shared_ptr<Expr> expr1;
        std::shared_ptr<Expr> expr2;

    public:
        Arith(Lexer::Token* tok, std::shared_ptr<Expr> x1, std::shared_ptr<Expr> x2) : Op(tok, nullptr), expr1(x1), expr2(x2)
        {
            type = Symbols::Type::max(expr1->type, expr2->type);
            if (!type) {
                error("type error");
            }
        }
        Expr* gen() { return new Arith(op, expr1->reduce(), expr2->reduce()); }

        std::string toString() const {
            std::ostringstream oss;
            oss << expr1->toString() << " "
                << op->toString() << " "
                << expr2->toString();
            return oss.str();
        }

    private:
        void error(const std::string& msg) {
            throw std::runtime_error(msg);
        }
    };
}
