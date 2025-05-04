#pragma once
#include "stmt.h"
#include "expr.h"
#include "Symbols/symbols.h"

namespace Inter
{
    class If : public Stmt 
    {
        Expr* expr;
        Stmt* stmt;
    public:
        If(Expr* x, Stmt* s) : expr(x), stmt(s) {
            if (expr->type != Symbols::Type::Bool) {
                expr->error("boolean required in if");
            }
        }

        void gen(int b, int a) {
            int label = newlabel();      // Ϊstmt�������ɱ�ǩ
            expr->jumping(0, a);         // trueʱ˳��ִ�У�falseʱ��ת��a
            emitlabel(label);            // �����ǩ
            stmt->gen(label, a);         // ����stmt�Ĵ���
        }
    };
}