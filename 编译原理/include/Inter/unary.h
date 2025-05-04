#pragma once

#include "op.h"
#include "expr.h"
#include "Lexer/Token.h"
#include "Symbols/symbols.h"

#include <string>
namespace Inter
{
    class Unary : public Op {
    public:
        Expr* expr;  

        Unary(Lexer::Token * tok, Expr* x) : Op(tok, nullptr), expr(x) {
            type = Symbols::Type::max(Symbols::Type::Int, expr->type);  // 计算最大兼容类型（如 int + float → float）
            if (type == nullptr) {
                error("type error");
            }
        }
        Expr* gen() { return new Unary(op, expr->reduce()); }
        std::string toString() const { return op->toString() + " " + expr->toString(); }
    private:
        void error(const std::string& msg) { throw std::runtime_error("Unary: " + msg); }
    };

}