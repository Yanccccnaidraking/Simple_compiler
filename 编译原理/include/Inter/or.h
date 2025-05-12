#pragma once
#include "logical.h"
#include "Lexer/Token.h"
#include "Symbols/symbols.h"

namespace Inter
{
    class Or : public Logical {
    public:
        Or(std::shared_ptr<Lexer::Token> tok, std::shared_ptr<Expr> x1, std::shared_ptr<Expr> x2) : Logical(tok, x1, x2) {}
        void jumping(int t, int f) {
            int label = t != 0 ? t : newlabel(); // t不是0就是确定的t，t是0创建新的
            expr1->jumping(label, 0); // ture出口对应t，false出口对应e2的结果
            expr2->jumping(t, f); 
            if (t == 0) {
                emitlabel(label);
            }
        }
    };
}