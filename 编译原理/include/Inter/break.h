#pragma once

#include"stmt.h"

namespace Inter
{
	class Break
	{
	public:
		Stmt stmt;
		Break();
		void gen(int b, int a);
	};
}