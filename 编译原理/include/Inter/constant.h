#pragma once


#include "expr.h"
#include "Lexer/Token.h" 
#include "Symbols/symbols.h"
#include<string.h>
namespace Inter
{
    class Constant : public Expr 
    {
        static Constant* True;
        static Constant* False;

        Constant* True = nullptr;
        Constant* False = nullptr;

        public:

        void emit(const std::string& code);

        void initConstants() {
            if (!Constant::True) {
                Constant::True = new Constant(Lexer::Word::true_, Symbols::Type::Bool);
                Constant::False = new Constant(Lexer::Word::false_, Symbols::Type::Bool);
            }
        }

        // ���캯��ʵ��
        Constant(Lexer::Token* tok, Symbols::Type* p) : Expr(tok, p) {}

        Constant(int i) : Expr(new Num(i), Symbols::Type::Int) {}

        // jumping����ʵ��
        void jumping(int t, int f) {
            if (this == True && t != 0) {
                emit("goto L" + std::to_string(t));
            }
            else if (this == False && f != 0) {
                emit("goto L" + std::to_string(f));
            }
        }
    };
}

