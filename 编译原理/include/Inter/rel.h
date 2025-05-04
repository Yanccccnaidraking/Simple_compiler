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
            // ����ϵͳ���
            if (dynamic_cast<Symbols::Array*>(p1) || dynamic_cast<Symbols::Array*>(p2)) {
                return nullptr;  // �������Ͳ�֧�ֱȽ�
            }

            if (typeid(*p1) == typeid(*p2)) {  // �����ϸ�ƥ��
                return Symbols::Type::Bool;             // ���ز�������
            }
            return nullptr;  // ���Ͳ�����
        }

        void jumping(int t, int f) {
            // �����м����
            Expr* a = expr1->reduce();  // ��Լ����ʽ
            Expr* b = expr2->reduce();  // ��Լ�ұ��ʽ

            // ���ɱȽϱ��ʽ�ַ���
            std::string test = a->toString() + " "
                + op->toString() + " "
                + b->toString();

            // ������ת����
            emitjumps(test, t, f);
        }
    };
}
