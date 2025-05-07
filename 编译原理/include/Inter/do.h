#pragma once

#include "stmt.h"
#include "expr.h"

namespace Inter
{
    class Do : public Stmt 
    {
        std::shared_ptr<Expr> expr;
        std::shared_ptr<Stmt> stmt;
     public:
        Do() : expr(nullptr), stmt(nullptr) {}

        void init(std::shared_ptr<Stmt> s, std::shared_ptr<Expr> x) {
            expr = x;
            stmt = s;
            if (expr->type != Symbols::Type::Bool) {
                expr->error("boolean required in do");
            }
            for (auto& s : stmt->Enclosing)
            {
                *s = *this;
            }
        }

        void gen(int b, int a) {
            *after = a;
            int label = newlabel();
            stmt->gen(b, label);
            emitlabel(label);
            expr->jumping(b, 0);
        }
    };

}