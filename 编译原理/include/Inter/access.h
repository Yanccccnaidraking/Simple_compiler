#pragma once
#include "op.h"
#include "id.h"
#include "expr.h"
#include "Symbols/symbols.h"

namespace Symbols {
    class Type;
}
namespace Inter
{
    class Access : public Op {
    public:
        Id* array;
        Expr* index;

        Access(Id* a, Expr* i, Symbols::Type* p);
        Expr* gen();
        void jumping(int t, int f);
        std::string toString();
    };
}

