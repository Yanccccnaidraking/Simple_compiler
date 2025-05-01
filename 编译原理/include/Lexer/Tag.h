#pragma once
#include<unordered_map>
namespace Lexer {
	/// 各词法单元对应的常数
	/// 其中 INDEX、MINUS、TEMP 不是词法单元，将在抽象语法树中使用
	enum class Tag : int
	{
		AND = 256, BASIC, BREAK, CASE, CONTINUE, DEFAULT, DO, ELSE, EQ, FALSE, FOR, GE,
		ID, IF, INDEX, LE, MINUS, NE, NUM, OR, REAL, RETURN, SWITCH, TEMP, TRUE, VOID, WHILE,
		PLUS_PLUS, MINUS_MINUS, PLUS_EQ, MINUS_EQ, MULT_EQ, DIV_EQ, MOD_EQ, AND_EQ, OR_EQ, XOR_EQ,
		LEFT_SHIFT, RIGHT_SHIFT, ARROW, STRING, CHAR, BOOL, END
	};

	inline std::string tagToString(Tag tag) {
		static const std::unordered_map<Tag, std::string> tagNameMap = {
			{Tag::AND, "&&"}, {Tag::BASIC, "basic"}, {Tag::BREAK, "break"},
			{Tag::CASE, "case"}, {Tag::CONTINUE, "continue"}, {Tag::DEFAULT, "default"},
			{Tag::DO, "do"}, {Tag::ELSE, "else"}, {Tag::EQ, "=="},
			{Tag::FALSE, "false"}, {Tag::FOR, "for"}, {Tag::GE, ">="},
			{Tag::ID, "id"}, {Tag::IF, "if"}, {Tag::INDEX, "index"},
			{Tag::LE, "<="}, {Tag::MINUS, "-"}, {Tag::NE, "!="},
			{Tag::NUM, "num"}, {Tag::OR, "||"}, {Tag::REAL, "real"},
			{Tag::RETURN, "return"}, {Tag::SWITCH, "switch"}, {Tag::TEMP, "temp"},
			{Tag::TRUE, "true"}, {Tag::VOID, "void"}, {Tag::WHILE, "while"},
			{Tag::PLUS_PLUS, "++"}, {Tag::MINUS_MINUS, "--"},
			{Tag::PLUS_EQ, "+="}, {Tag::MINUS_EQ, "-="},
			{Tag::MULT_EQ, "*="}, {Tag::DIV_EQ, "/="},
			{Tag::MOD_EQ, "%="}, {Tag::AND_EQ, "&="}, {Tag::OR_EQ, "|="},
			{Tag::XOR_EQ, "^="}, {Tag::LEFT_SHIFT, "<<"},
			{Tag::RIGHT_SHIFT, ">>"}, {Tag::ARROW, "->"},
			{Tag::STRING, "string"}, {Tag::CHAR, "char"}, {Tag::BOOL, "bool"}, {Tag::END,"$"}
		};

		auto it = tagNameMap.find(tag);
		if (it != tagNameMap.end()) {
			return it->second;
		}
		else {
			return std::string(1, static_cast<char>(tag));//将字符本身返回
		}
	}
}
