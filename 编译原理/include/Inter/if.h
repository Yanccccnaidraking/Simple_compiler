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
            int label = newlabel();      // 为stmt代码生成标签
            expr->jumping(0, a);         // true时顺序执行，false时跳转到a
            emitlabel(label);            // 发射标签
            stmt->gen(label, a);         // 生成stmt的代码
        }
    };
}