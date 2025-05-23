#pragma once
#include "stmt.h"

namespace Inter
{
    class Seq : public Stmt {
    private:
        //Stmt* stmt1;
        std::shared_ptr<Stmt> stmt1;
        //Stmt* stmt2;
        std::shared_ptr<Stmt> stmt2;


    public:
        //Seq(Stmt* s1, Stmt* s2) : stmt1(s1), stmt2(s2) {}
        Seq(std::shared_ptr<Stmt> s1, std::shared_ptr<Stmt> s2) : stmt1(s1), stmt2(s2) {
            Enclosing = s1->Enclosing;
            Enclosing.insert(s2->Enclosing.begin(), s2->Enclosing.end());
        }
        void gen(int b, int a) {
            if (stmt1 == Stmt::Null) {
                stmt2->gen(b, a);
            }
            else if (stmt2 == Stmt::Null) {
                stmt1->gen(b, a);
            }
            else {
                int label = newlabel();
                stmt1->gen(b, label);
                emitlabel(label);
                stmt2->gen(label, a);
            }
        }
    };
}