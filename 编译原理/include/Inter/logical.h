#pragma once

#include "expr.h"
#include "temp.h"
#include "Lexer/Token.h" 
#include "Symbols/symbols.h"
#include <memory>
#include <sstream>

namespace Inter
{
    class Logical : public Expr {
    protected:
        //Expr* expr1;
        std::shared_ptr<Expr> expr1;
        //Expr* expr2;
        std::shared_ptr<Expr> expr2;

    public:
        /*Logical(Lexer::Token* tok, Expr* x1, Expr* x2) : Expr(tok, nullptr), expr1(x1), expr2(x2)
        {
            type = check(expr1->type, expr2->type);
            if (!type) error("Type error");
        }*/
        Logical(std::shared_ptr<Lexer::Token> tok, std::shared_ptr<Expr> x1, std::shared_ptr<Expr> x2) : Expr(tok, nullptr), expr1(x1), expr2(x2)
        {
            type = check(expr1->type, expr2->type);
            if (!type) error("Type error");
        }

        static Symbols::Type* check(Symbols::Type* p1, Symbols::Type* p2)
        {
            if (p1 == Symbols::Type::Bool && p2 == Symbols::Type::Bool) {
                return Symbols::Type::Bool;
            }
            return nullptr;
        }

        /*Expr* gen() 
        {
            int f = newlabel();
            int a = newlabel();
            Temp* temp = new Temp(type);

            this->jumping(0, f);
            emit(temp->toString() + " = true");
            emit("goto L" + std::to_string(a));

            emitlabel(f);
            emit(temp->toString() + " = false");

            emitlabel(a);
            return temp;
        }*/
        shared_ptr<Expr> gen()
        {
            int f = newlabel();
            int a = newlabel();
            shared_ptr<Temp> temp = make_shared<Temp>(type);

            this->jumping(0, f);
            emit(temp->toString() + " = true");
            emit("goto L" + std::to_string(a));

            emitlabel(f);
            emit(temp->toString() + " = false");

            emitlabel(a);
            return temp;
        }
        std::string toString() const {
            std::ostringstream oss;
            oss << expr1->toString() << " "
                << op->toString() << " "
                << expr2->toString();
            return oss.str();
        }

    private:
        void error(const std::string& msg) {
            throw std::runtime_error(msg);
        }
    };
}
