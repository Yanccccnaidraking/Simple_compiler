#pragma once
#include "stmt.h"
#include "expr.h"
#include "Symbols/symbols.h"

namespace Inter
{
    class Else : public Stmt 
    {
        std::shared_ptr<Expr> expr;
        std::shared_ptr<Stmt> stmt1;
        std::shared_ptr<Stmt> stmt2;

    public:

        Else(std::shared_ptr<Expr> x, std::shared_ptr<Stmt> s1, std::shared_ptr<Stmt> s2) : expr(x), stmt1(s1), stmt2(s2)
        {
            if (expr->type != Symbols::Type::Bool)
                expr->error("boolean required in if");
            Enclosing = stmt1->Enclosing;
            Enclosing.insert(stmt2->Enclosing.begin(), stmt2->Enclosing.end());
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