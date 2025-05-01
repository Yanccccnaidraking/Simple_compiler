#pragma once
#include "logical.h"
#include "Lexer/Token.h"
#include "Symbols/symbols.h"

namespace Inter
{
    class Or : public Logical {
    public:
        Or(Lexer::Token* tok, Expr* x1, Expr* x2);
        void jumping(int t, int f) ;
    };
}