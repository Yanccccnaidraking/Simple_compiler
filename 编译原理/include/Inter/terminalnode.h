#pragma once
#include "Inter/node.h"
#include "Lexer/Token.h"

namespace Inter {
	/// <summary>
	/// ���ⶨ��һ����������״̬ת�Ƶ�ʱ��洢��Ӧ��token���Ա��ڹ�Լ��ʱ��ֱ��ʹ��
	/// </summary>
	class TerminalNode :public Node {
	public:
		std::shared_ptr<Lexer::Token> token;
		TerminalNode(std::shared_ptr<Lexer::Token> t) :token(t) {}
	};
}