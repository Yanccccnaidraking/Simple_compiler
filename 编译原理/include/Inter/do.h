#pragma once

#include "stmt.h"
#include "expr.h"

namespace Inter
{
    class Do : public Stmt 
    {
        Expr* expr;
        Stmt* stmt;
     public:
        Do() : expr(nullptr), stmt(nullptr) {}

        void init(Stmt* s, Expr* x) {
            expr = x;
            stmt = s;
            if (expr->type != Symbols::Type::Bool) {
                expr->error("boolean required in do");
            }
        }

        void gen(int b, int a) {
            after = a;
            int label = newlabel();
            stmt->gen(b, label);
            emitlabel(label);
            expr->jumping(b, 0);
        }
    };

}