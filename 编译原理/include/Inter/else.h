#pragma once
#include "stmt.h"
#include "expr.h"
#include "Symbols/symbols.h"

namespace Inter
{
    class Else : public Stmt 
    {
        Expr* expr;
        Stmt* stmt1;
        Stmt* stmt2;

    public:
        Else(Expr* x, Stmt* s1, Stmt* s2) : expr(x), stmt1(s1), stmt2(s2)
        {
            if (expr->type != Symbols::Type::Bool) 
                expr->error("boolean required in if");
        }

        void gen(int b, int a){
            int label1 = newlabel();  // stmt1的标签
            int label2 = newlabel();  // stmt2的标签

            // 表达式为true时顺序执行，false时跳转到label2
            expr->jumping(0, label2);

            // 生成stmt1的代码
            emitlabel(label1);
            stmt1->gen(label1, a);
            emit("goto L" + std::to_string(a));  // 执行完stmt1后跳转到结束标签

            // 生成stmt2的代码
            emitlabel(label2);
            stmt2->gen(label2, a);
        }
    };

}