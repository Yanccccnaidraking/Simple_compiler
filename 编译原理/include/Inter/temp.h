#pragma once
#include "expr.h"
#include "Symbols/symbols.h"
#include "Lexer/lexer.h"

namespace Inter {
    class Temp : public Expr {
    public:
        static int count;
        int number;
        Temp(std::shared_ptr<Symbols::Type> p) : Expr(Lexer::Word::temp, p), number(++count) {}
        std::string toString() const {
            return "t" + std::to_string(number);
        }
    };
}
