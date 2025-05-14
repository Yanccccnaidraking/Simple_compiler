#pragma once
#include "stmt.h"

namespace Inter {
	class Continue: public Stmt
	{
		std::shared_ptr<Stmt> stmt;
	public:
		Continue() {
			stmt = Stmt::Null;
		}
		void gen(int b, int a) { emit("goto L" + std::to_string(*(stmt->begin))); };
	};
}