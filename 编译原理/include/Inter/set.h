#pragma once
#include "stmt.h"
#include "id.h"
#include "Symbols/symbols.h"

namespace Inter
{
    class Set : public Stmt {
    public:
        Id* id;
        Expr* expr;

        Set(Id* i, Expr* x);
        Symbols::Type* check(Symbols::Type* p1, Symbols::Type* p2);
        void gen(int b, int a) ;
        void error(const std::string& msg);
    };
}