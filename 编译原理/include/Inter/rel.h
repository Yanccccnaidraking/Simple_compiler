#pragma once

#include"excpt.h"
#include "logical.h"
#include "Lexer/token.h"
#include "Symbols/symbols.h"
namespace Inter
{
    class Rel : public Logical {
    public:
        Rel(Lexer::Token* tok, Expr* x1, Expr* x2);
        Symbols::Type* check(Symbols::Type* p1, Symbols::Type* p2) ;
        void jumping(int t, int f) ;
    };
}
