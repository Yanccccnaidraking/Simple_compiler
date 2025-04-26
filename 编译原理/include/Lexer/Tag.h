#pragma once
namespace Lexer {
	/// ���ʷ���Ԫ��Ӧ�ĳ���
	/// ���� INDEX��MINUS��TEMP ���Ǵʷ���Ԫ�����ڳ����﷨����ʹ��
	enum class Tag : int
	{
		AND = 256, BASIC, BREAK, CASE, CONTINUE, DEFAULT, DO, ELSE, EQ, FALSE, FOR, GE,
		ID, IF, INDEX, LE, MINUS, NE, NUM, OR, REAL, RETURN, SWITCH, TEMP, TRUE, VOID, WHILE,
		PLUS_PLUS, MINUS_MINUS, PLUS_EQ, MINUS_EQ, MULT_EQ, DIV_EQ, MOD_EQ, AND_EQ, OR_EQ, XOR_EQ,
		LEFT_SHIFT, RIGHT_SHIFT, ARROW, STRING, CHAR, BOOL
	};
}
