#pragma once
#include "Inter/node.h"
#include "Symbols/symbols.h"

namespace Inter {
	/// <summary>
	/// ����洢�������͵Ľ��
	/// </summary>
	class TypeNode :public Node {
	public:
		std::shared_ptr<Symbols::Type> type;
		TypeNode(std::shared_ptr<Symbols::Type> t) :type(t) {}
	};
}