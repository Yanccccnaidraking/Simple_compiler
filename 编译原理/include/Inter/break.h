#pragma once

#include"stmt.h"

namespace Inter
{
	class Break
	{
		Stmt* stmt;

	public:
		
		void error(const std::string& msg);
		void emit(const std::string& code);

		Break() 
		{
			if (Stmt::Enclosing == Stmt::Null) { error("unenclosed break"); }
			stmt = Stmt::Enclosing;
		}

		void gen(int b, int a) { emit("goto L" + std::to_string(stmt->after)); };
	};
}