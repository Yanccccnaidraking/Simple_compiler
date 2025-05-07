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
        Rel(std::shared_ptr<Lexer::Token> tok, std::shared_ptr<Expr> x1, std::shared_ptr<Expr> x2) : Logical(tok, x1, x2,true) {}

        std::shared_ptr<Symbols::Type> check(std::shared_ptr<Symbols::Type> p1, std::shared_ptr<Symbols::Type> p2) {
            // ����ϵͳ���
            if (dynamic_pointer_cast<Symbols::Array>(p1) || dynamic_pointer_cast<Symbols::Array>(p2)) {
                return nullptr;  // �������Ͳ�֧�ֱȽ�
            }

            if (typeid(*p1) == typeid(*p2)) {  // �����ϸ�ƥ��
                return Symbols::Type::Bool;             // ���ز�������
            }
            return nullptr;  // ���Ͳ�����
        }

        void jumping(int t, int f) {
            // �����м����
            std::shared_ptr<Expr> a = expr1->reduce();  // ��Լ����ʽ
            std::shared_ptr<Expr> b = expr2->reduce();  // ��Լ�ұ��ʽ

            // ���ɱȽϱ��ʽ�ַ���
            std::string test = a->toString() + " "
                + op->toString() + " "
                + b->toString();

            // ������ת����
            emitjumps(test, t, f);
        }
    };
}
