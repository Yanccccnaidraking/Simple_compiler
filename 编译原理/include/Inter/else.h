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
            int label1 = newlabel();  // stmt1�ı�ǩ
            int label2 = newlabel();  // stmt2�ı�ǩ

            // ���ʽΪtrueʱ˳��ִ�У�falseʱ��ת��label2
            expr->jumping(0, label2);

            // ����stmt1�Ĵ���
            emitlabel(label1);
            stmt1->gen(label1, a);
            emit("goto L" + std::to_string(a));  // ִ����stmt1����ת��������ǩ

            // ����stmt2�Ĵ���
            emitlabel(label2);
            stmt2->gen(label2, a);
        }
    };

}