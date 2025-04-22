#pragma once
namespace Lexer {
	/// 各词法单元对应的常数
	/// 其中 INDEX、MINUS、TEMP 不是词法单元，将在抽象语法树中使用
	enum class Tag : int
	{
		AND = 256, BASIC, BREAK, CASE, DEFAULT, DO, ELSE, EQ, FALSE, FOR, GE,
		ID, IF, INDEX, LE, MINUS, NE, NUM, OR, REAL, SWITCH, TEMP, TRUE, WHILE
	};
}
