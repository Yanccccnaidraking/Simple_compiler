#pragma once

#include "stmt.h"
#include "expr.h"
#include "Symbols/symbols.h"
namespace Inter
{
    class While : public Stmt {
    public:
        shared_ptr<Expr> expr;
        shared_ptr<Stmt> stmt;
        int after;

        While() {};

        void init(shared_ptr<Expr> x, shared_ptr<Stmt> s) {
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