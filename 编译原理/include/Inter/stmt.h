#pragma once
#include "node.h"

namespace Inter {
	class Stmt : public Node 
	{
     public:
        static std::shared_ptr<Stmt> Null;
        static std::shared_ptr<Stmt> Enclosing;// used for break stmts

        int after ; // saves label after

        Stmt() : after(0){}
        void gen(int b, int a) { }// 基类实现，可由子类覆盖
    };

    std::shared_ptr<Stmt> Stmt::Null = std::make_shared<Stmt>();
    std::shared_ptr<Stmt> Stmt::Enclosing = Stmt::Null; 

}

