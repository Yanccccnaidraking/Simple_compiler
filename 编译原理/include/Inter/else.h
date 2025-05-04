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