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
        /// ����ʵ�֣��������า��
        /// </summary>
        /// <param name="b">begin�Ǳ��������Ŀ�ʼ��</param>
        /// <param name="a">after���������֮��ĵ�һ��ָ��</param>
        virtual void gen(int b, int a) { }
    };
}

