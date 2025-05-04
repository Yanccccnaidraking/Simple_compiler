#pragma once
#include "logical.h"
#include "Lexer/Token.h"
#include "Symbols/symbols.h"

namespace Inter
{
    class Or : public Logical {
    public:
        Or(Lexer::Token* tok, Expr* x1, Expr* x2) : Logical(tok, x1, x2) {}
        void jumping(int t, int f) {
            int label = t != 0 ? t : newlabel();
            expr1->jumping(label, 0);
            expr2->jumping(t, f);
            if (t == 0) {
                emitlabel(label);
            }
        }
    };
}