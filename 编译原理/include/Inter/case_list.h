#pragma once
#include "Inter/case.h"

namespace Inter {
	class CaseList : public Stmt {
	public:
		vector<std::shared_ptr<Inter::Case>> list;
		CaseList() {}
		void addCase(std::shared_ptr<Inter::Case> c)
		{
			list.push_back(c);
			Enclosing = c->Enclosing;
		}
	};
}