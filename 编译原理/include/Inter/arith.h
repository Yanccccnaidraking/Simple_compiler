#pragma once

#include "Op.h"
#include "expr.h"
#include "Lexer/Token.h" 
#include "Symbols/symbols.h"

namespace Inter
{
    class Arith : public Op {
    protected:
        Expr* expr1;
        Expr* expr2;

    public:
        Arith(Lexer::Token* tok, Expr* x1, Expr* x2);
        Expr* gen();
        std::string toString();

    private:
        void error(const std::string& msg);
    };
}
