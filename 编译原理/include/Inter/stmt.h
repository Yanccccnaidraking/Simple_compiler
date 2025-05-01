#pragma once
#include "node.h"

namespace Inter {
	class Stmt : public Node 
	{
     public:
        static Stmt* Enclosing;
        static Stmt* Null;

        int after;

        Stmt();
        void gen(int b, int a);
	};
}