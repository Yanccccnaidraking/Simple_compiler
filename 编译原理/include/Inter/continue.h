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
		void gen(int b, int a) { emit("goto L" + std::to_string(*(stmt->begin))); };
	};
}