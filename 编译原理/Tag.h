#pragma once
namespace Lexer {
	/// ���ʷ���Ԫ��Ӧ�ĳ���
	/// ���� INDEX��MINUS��TEMP ���Ǵʷ���Ԫ�����ڳ����﷨����ʹ��
	enum class Tag : int
	{
		AND = 256, BASIC, BREAK, DO, ELSE, EQ, FALSE, FOR, GE,
		ID, IF, INDEX, LE, MINUS, NE, NUM, OR, REAL, TEMP, TRUE, WHILE
	};
}
