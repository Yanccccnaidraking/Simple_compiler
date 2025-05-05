#pragma once

#include "logical.h"
#include "expr.h"
#include "Lexer/Token.h" 

namespace Inter
{
    class And : public Logical {
        public:
        //And(Lexer::Token* tok, Expr* x1, Expr* x2) : Logical(tok, x1, x2) {}
        And(std::shared_ptr<Lexer::Token> tok, std::shared_ptr<Expr> x1, std::shared_ptr<Expr> x2) : Logical(tok, x1, x2) {}
        void jumping(int t, int f)
        {
            int label = f != 0 ? f : newlabel();
            expr1->jumping(0, label);
            expr2->jumping(t, f);
            if (f == 0) {
                emitlabel(label);
            }
        }
    };
}

