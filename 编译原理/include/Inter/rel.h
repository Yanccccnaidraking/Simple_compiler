#pragma once

#include"excpt.h"
#include "logical.h"
#include "Lexer/token.h"
#include "Symbols/symbols.h"
#include <typeinfo>

namespace Inter
{
    class Rel : public Logical {
    public:
        Rel(Lexer::Token* tok, Expr* x1, Expr* x2) : Logical(tok, x1, x2) {}

        Symbols::Type* check(Symbols::Type* p1, Symbols::Type* p2) {
            // 类型系统检查
            if (dynamic_cast<Symbols::Array*>(p1) || dynamic_cast<Symbols::Array*>(p2)) {
                return nullptr;  // 数组类型不支持比较
            }

            if (typeid(*p1) == typeid(*p2)) {  // 类型严格匹配
                return Symbols::Type::Bool;             // 返回布尔类型
            }
            return nullptr;  // 类型不兼容
        }

        void jumping(int t, int f) {
            // 生成中间代码
            Expr* a = expr1->reduce();  // 规约左表达式
            Expr* b = expr2->reduce();  // 规约右表达式

            // 生成比较表达式字符串
            std::string test = a->toString() + " "
                + op->toString() + " "
                + b->toString();

            // 生成跳转代码
            emitjumps(test, t, f);
        }
    };
}
