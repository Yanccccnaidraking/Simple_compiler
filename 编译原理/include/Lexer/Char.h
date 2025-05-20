#pragma once
#include "Token.h"
#include "Tag.h"
#include <sstream>
namespace Lexer {
	// 类 Char 用于处理字符
	class Char : public Token
	{
	public:
		const char value;
		const std::string string_value;
		Char(std::string v) :  string_value(v), value(transformToChar(v)), Token(Tag::CHAR) {}

		//std::string toString() const { return string_value; }
		std::string toString() const { return  std::string(1, value);}

        inline char transformToChar(std::string s) {
            char c;
            // 移除单引号
            if (s.length() >= 2 && s[0] == '\'' && s[s.length() - 1] == '\'') {
                s = s.substr(1, s.length() - 2);
            }

            // 处理转义字符
            if (s[0] == '\\') {
                if (s.length() > 1) {
                    if (s[1] == 'x') { // Hexadecimal escape sequence
                        if (s.length() > 2) {
                            int hexValue;
                            std::stringstream ss;
                            ss << std::hex << s.substr(2, 2); // Only take the first two characters after \x
                            ss >> hexValue;
                            c = static_cast<char>(hexValue);
                        }
                        else {
                            throw std::invalid_argument("Invalid hexadecimal escape sequence.");
                        }
                    }
                    else if (isdigit(s[1])) { // Octal escape sequence
                        int octValue = 0;
                        for (size_t i = 1; i < s.length() && i <= 3; ++i) { // Octal is max 3 digits
                            if (!isdigit(s[i]) || s[i] > '7') {
                                throw std::invalid_argument("Invalid octal escape sequence.");
                            }
                            octValue = octValue * 8 + (s[i] - '0');
                        }
                        c = static_cast<char>(octValue);
                    }
                    else { // Other escape sequences
                        if (s == "\\n") c = '\n';
                        else if (s == "\\t") c = '\t';
                        else if (s == "\\r") c = '\r';
                        else if (s == "\\a") c = '\a';
                        else if (s == "\\b") c = '\b';
                        else if (s == "\\f") c = '\f';
                        else if (s == "\\v") c = '\v';
                        else if (s == "\\\\") c = '\\';
                        else if (s == "\\\'") c = '\'';
                        else if (s == "\\\"") c = '\"';
                        else {
                            throw std::runtime_error("Unsupported escape sequence.");
                        }
                    }
                }
                else {
                    throw std::runtime_error("Invalid escape sequence.");
                }
            }
            else if (s.length() == 1) { // Single character
                c = s[0];
            }
            else {
                throw std::runtime_error("empty character constant. Primitive character : " + s);
            }

            return c;
        }
	};
}