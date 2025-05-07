#pragma once
#include "Inter/node.h"
#include "Symbols/symbols.h"

namespace Inter {
	/// <summary>
	/// 定义存储变量类型的结点
	/// </summary>
	class TypeNode :public Node {
	public:
		std::shared_ptr<Symbols::Type> type;
		TypeNode(std::shared_ptr<Symbols::Type> t) :type(t) {}
	};
}