#pragma once

#include "logical.h"
#include "expr.h"
#include "Lexer/Token.h" 

namespace Inter
{
    class And : public Logical {
        public:
        And(Lexer::Token* tok, Expr* x1, Expr* x2);
        virtual void jumping(int t, int f) ;
    };
}

