#pragma once

#include "stmt.h"
#include "expr.h"
#include "Symbols/symbols.h"
namespace Inter
{
    class While : public Stmt {
    public:
        Expr* expr;
        Stmt* stmt;
        int after;

        While();
        void init(Expr* x, Stmt* s) {
            expr = x;
            stmt = s;
            if (expr->type != Symbols::Type::Bool) {
                error("boolean required in while");
            }
        }

        void gen(int b, int a){
            after = a;
            expr->jumping(0, a);

            int label = newlabel();
            emitlabel(label);
            stmt->gen(label, b);
            emit("goto L" + std::to_string(b));
        }

    private:
        void error(const std::string& msg) {
            throw std::runtime_error("While: " + msg);
        }
    };

}