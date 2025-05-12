#pragma once

#include "logical.h"
#include "expr.h"
#include "Lexer/Token.h" 

namespace Inter
{
    class And : public Logical {
        public:
        And(std::shared_ptr<Lexer::Token> tok, std::shared_ptr<Expr> x1, std::shared_ptr<Expr> x2) : Logical(tok, x1, x2) {}
        void jumping(int t, int f)
        {
            int label = f != 0 ? f : newlabel();// ��� f ��Ϊ 0����ʹ�� f ��Ϊ��ת��ǩ����������һ���±�ǩ
            expr1->jumping(0, label);// ��� expr1 Ϊ false����ת�� label
            expr2->jumping(t, f); // ��� expr1 Ϊ true��ֱ�Ӵ�Խ���� expr2 ����ת
            if (f == 0) {
                emitlabel(label); // ��� f == 0������λ�� label �����̬��ǩ
            }
        }
    };
}

