#pragma once
#include "node.h"
#include<unordered_set>
namespace Inter {
	class Stmt : public Node 
	{
     public:
        std::unordered_set<std::shared_ptr<Stmt>> Enclosing;// used for break stmts
        static std::shared_ptr<Stmt> Null;
        //static std::shared_ptr<Stmt> Enclosing;// used for break stmts

        std::shared_ptr<int> after ; // saves label after

        Stmt() {
            after = std::make_shared<int>(0);
        }
        virtual void gen(int b, int a) { }// 基类实现，可由子类覆盖
    };
}

