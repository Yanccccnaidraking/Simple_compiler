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
        std::shared_ptr<int> begin;

        Stmt() {
            after = std::make_shared<int>(0);
            begin = std::make_shared<int>(0);
        }
        /// <summary>
        /// 基类实现，可由子类覆盖
        /// </summary>
        /// <param name="b">begin是标记语句代码的开始处</param>
        /// <param name="a">after标记语句代码之后的第一条指令</param>
        virtual void gen(int b, int a) { }
    };
}

