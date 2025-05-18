#pragma once
#include "Inter/stmt.h"
#include "Inter/case_list.h"
#include "Inter/expr.h"
namespace Inter {
	class Switch:public Stmt 
	{
		std::shared_ptr<Inter::Expr> expr;
		std::shared_ptr<Inter::CaseList> list;
		std::shared_ptr<Inter::Case> def;
	public:
		Switch(std::shared_ptr<Inter::Expr> e,std::shared_ptr<Inter::CaseList> list,std::shared_ptr<Inter::Case> def): expr(e),list(list),def(def)
		{
			for (auto& s : list->Enclosing)
			{
				*s = *this;
			}
			if (def)
			{
				for (auto& s : def->Enclosing)
				{
					*s = *this;
				}
			}
		}

		void gen(int b,int a)
		{
			*after = a;
			*begin = b;
			int size = list->list.size();
			vector<int> labels;
			for (int i = 0; i < size; i++)
			{
				labels.push_back(newlabel());
			}
			int defLabel;
			if (def)
			{
				defLabel = newlabel();
			}
			vector<std::shared_ptr<Inter::Expr>> exprs;
			std::shared_ptr<Inter::Expr> temp = expr->reduce();
			for (int i = 0; i < size; i++)
			{
				exprs.push_back(list->list[i]->reduce());//首先将所有的case语句的判断值计算出来
			}
			for (int i = 0; i < size; i++)
			{
				list->list[i]->emitCase(temp->toString(), exprs[i]->toString(), labels[i]);
			}
			if (def)
			{
				def->emitCase(temp->toString(), temp->toString(), defLabel);
			}
			for (int i = 0; i < size; i++)
			{
				emitlabel(labels[i]);
				list->list[i]->gen(0, a);
			}
			if (def)
			{
				emitlabel(defLabel);
				def->gen(0,a);
			}
		}
	};
}