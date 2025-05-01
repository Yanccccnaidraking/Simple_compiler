#pragma once

#include "op.h"
#include "expr.h"
#include "Lexer/Token.h"
#include "Symbols/symbols.h"

#include <string>
namespace Inter
{
    class Unary : public Op {
    public:
        Expr* expr;  
        Unary(Lexer::Token* tok, Expr* x);  
        Expr* gen() ;        
        std::string toString();
    private:
        void error(const std::string& msg);
    };

}