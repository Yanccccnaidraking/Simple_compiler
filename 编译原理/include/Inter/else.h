#pragma once
#include "stmt.h"
#include "expr.h"
#include "Symbols/symbols.h"

namespace Inter
{
    class Else : public Stmt 
    {
        Expr* expr;
        Stmt* stmt1;
        Stmt* stmt2;

    public:
        Else(Expr* x, Stmt* s1, Stmt* s2);
        void gen(int b, int a);
    };

}