#pragma once
#include "expr.h"
#include "Symbols/symbols.h"
#include "Lexer/lexer.h"

namespace Inter {
    class Temp : public Expr {
    private:
        static int count;
        int number;
    public:
        Temp(Symbols::Type* p) : Expr(Lexer::Word::temp.get(), p), number(++count) {}
        std::string toString() const {
            return "t" + std::to_string(number);
        }
    };
}
