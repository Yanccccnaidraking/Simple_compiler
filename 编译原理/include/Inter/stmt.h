#pragma once
#include "node.h"

namespace Inter {
	class Stmt : public Node 
	{
     public:
        static Stmt* Enclosing;
        static Stmt* Null;
        Stmt* Enclosing = nullptr;
        Stmt* Null = nullptr;
        int after;

        Stmt() : after(0) {}
        void gen(int b, int a) { }// ����ʵ�֣��������า��
	};
}