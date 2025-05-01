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
        Do();
        void init(Stmt* s, Expr* x);
        void gen(int b, int a) ;
    };

}