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
			stmt = Stmt::Null;
			Enclosing.insert(stmt);
		}

		void gen(int b, int a) { emit("goto L" + std::to_string(*(stmt->after))); };
	};
}