#pragma once

#include"stmt.h"

namespace Inter
{
	class Break: public Stmt
	{
		std::shared_ptr<Stmt> stmt;

	public:
		
		Break() 
		{
			if (Stmt::Enclosing == Stmt::Null) { error("unenclosed break"); }
			stmt = Stmt::Enclosing;
		}

		void gen(int b, int a) { emit("goto L" + std::to_string(stmt->after)); };
	};
}