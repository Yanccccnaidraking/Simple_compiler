#pragma once

#include "expr.h"
#include "Lexer/Token.h" 
#include "Lexer/Num.h"
#include "Symbols/symbols.h"
#include<string.h>
namespace Inter
{
    class Constant : public Expr 
    {
    public:
        static shared_ptr<Constant> True;
        static shared_ptr<Constant> False;

        /*void initConstants() {
            if (!Constant::True) {
                Constant::True = make_shared<Constant>(Lexer::Word::true_, Symbols::Type::Bool);
                
            }
            if (!Constant::False) {
                Constant::False = make_shared<Constant>(Lexer::Word::false_, Symbols::Type::Bool);
            }
        }*/

        // ���캯��ʵ��
        Constant(std::shared_ptr<Lexer::Token> tok, std::shared_ptr<Symbols::Type> p) : Expr(tok, p) {}

        Constant(int i) : Expr(std::make_shared<Lexer::Num>(i), Symbols::Type::Int) {}

        // jumping����ʵ��
        void jumping(int t, int f) {
            if (this == True.get() && t != 0) {
                emit("goto L" + std::to_string(t));
            }
            else if (this == False.get() && f != 0) {
                emit("goto L" + std::to_string(f));
            }
        }
    };
}

