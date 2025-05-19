#pragma once
#include "stmt.h"

namespace Inter {
	class Continue: public Stmt
	{
		std::shared_ptr<Stmt> stmt;
	public:
		Continue() {
			stmt = Stmt::Null;
			Enclosing.insert(stmt);
		}
		void gen(int b, int a) { 
			if (*(stmt->begin)==0)
			{
				error("continue语句必须在循环或switch语句内");
			}
			emit("goto L" + std::to_string(*(stmt->begin))); 
		}
	};
}