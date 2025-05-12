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
            int label = f != 0 ? f : newlabel();// 如果 f 不为 0，则使用 f 作为跳转标签，否则生成一个新标签
            expr1->jumping(0, label);// 如果 expr1 为 false，跳转到 label
            expr2->jumping(t, f); // 如果 expr1 为 true，直接穿越计算 expr2 的跳转
            if (f == 0) {
                emitlabel(label); // 如果 f == 0，则在位置 label 输出动态标签
            }
        }
    };
}

