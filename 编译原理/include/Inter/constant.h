#pragma once

#include "expr.h"
#include "Lexer/Token.h" 
#include "Symbols/symbols.h"
namespace Inter
{
    class Constant : public Expr {
    public:
        // ��̬������ָ��ʵ�ֵ�����
        static Constant* True;
        static Constant* False;

        // ���캯��
        Constant(Lexer::Token* tok, Symbols::Type * p);
        Constant(int i);

        // ����
        void jumping(int t, int f);
    };
}

