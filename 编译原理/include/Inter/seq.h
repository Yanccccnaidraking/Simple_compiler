#pragma once
#include "stmt.h"

namespace Inter
{
    class Seq : public Stmt {
    private:
        Stmt* stmt1;
        Stmt* stmt2;

    public:
        Seq(Stmt* s1, Stmt* s2);
        void gen(int b, int a) ;
    };
}