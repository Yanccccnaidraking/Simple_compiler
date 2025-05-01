#pragma once

#include "stmt.h"
#include "id.h"
#include "access.h"
#include "Symbols/symbols.h"

namespace Inter
{
    class SetElem : public Stmt {
    public:
        Id* array;
        Expr* index;
        Expr* expr;

        SetElem(Access* x, Expr* y);
        Symbols::Type* check(Symbols::Type * p1, Symbols::Type* p2);
        void gen(int b, int a);

    private:
        void error(const std::string& msg);
    };
}