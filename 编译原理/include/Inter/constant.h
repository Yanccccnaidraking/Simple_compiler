#pragma once

#include "expr.h"
#include "Lexer/Token.h" 
#include "Symbols/symbols.h"
namespace Inter
{
    class Constant : public Expr {
    public:
        // 静态常量（指针实现单例）
        static Constant* True;
        static Constant* False;

        // 构造函数
        Constant(Lexer::Token* tok, Symbols::Type * p);
        Constant(int i);

        // 方法
        void jumping(int t, int f);
    };
}

