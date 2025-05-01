#pragma once
#include "stmt.h"
#include "expr.h"
#include "Symbols/symbols.h"

namespace Inter
{
    class If : public Stmt 
    {
        Expr* expr;
        Stmt* stmt;
    public:
        If(Expr* x, Stmt* s);
        void gen(int b, int a);
    };
}