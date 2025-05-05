#pragma once
#include "Inter/node.h"
#include "Lexer/Token.h"

namespace Inter {
	/// <summary>
	/// 额外定义一个类用于在状态转移的时候存储对应的token，以便在规约的时候直接使用
	/// </summary>
	class TerminalNode :public Node {
	public:
		std::shared_ptr<Lexer::Token> token;
		TerminalNode(std::shared_ptr<Lexer::Token> t) :token(t) {}
	};
}