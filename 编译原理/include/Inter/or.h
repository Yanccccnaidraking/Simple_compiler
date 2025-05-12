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
            int label = t != 0 ? t : newlabel(); // t����0����ȷ����t��t��0�����µ�
            expr1->jumping(label, 0); // ture���ڶ�Ӧt��false���ڶ�Ӧe2�Ľ��
            expr2->jumping(t, f); 
            if (t == 0) {
                emitlabel(label);
            }
        }
    };
}