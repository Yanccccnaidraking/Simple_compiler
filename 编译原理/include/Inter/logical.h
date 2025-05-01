#pragma once

#include "expr.h"
#include "temp.h"
#include "Lexer/Token.h" 
#include "Symbols/symbols.h"
#include <memory>

namespace Inter
{
    class Logical : public Expr {
    protected:
        Expr* expr1;
        Expr* expr2;

    public:
        Logical(Lexer::Token* tok, Expr* x1, Expr* x2);

        static Symbols::Type* check(Symbols::Type* p1, Symbols::Type* p2);

        Expr* gen() ;
        std::string toString() ;

    private:
        void error(const std::string& msg);
    };
}
