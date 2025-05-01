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
        void init(Expr* x, Stmt* s);
        void gen(int b, int a);

    private:
        void error(const std::string& msg);
    };

}