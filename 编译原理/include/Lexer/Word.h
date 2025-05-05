#pragma once
#include "Token.h"
#include "Tag.h"
#include <string>

namespace Lexer {
    // 类 Word 用于管理保留字、标识符和项 && 这样的复合词法单元的词素
    // 也用来管理在中间代码中运算符的书写形式
    class Word : public Token
    {
    public:
        std::string lexeme = "";

        Word() :lexeme(""), Token() {}
        Word(const std::string& s, int tag) : lexeme(s), Token(tag) {}
        Word(const std::string& s, Tag tag) : lexeme(s), Token(tag) {}

        //// 显式定义特殊成员函数
        //Word(const Word&) = default;
        //Word& operator=(const Word&) = delete;  // 禁用拷贝赋值
        //Word(Word&&) = default;                // 允许移动构造
        //Word& operator=(Word&&) = delete;      // 禁用移动赋值


        std::string toString() const { return lexeme; }

        static const std::shared_ptr<Word> and_;
        static const std::shared_ptr<Word> or_;
        static const std::shared_ptr<Word> eq;
        static const std::shared_ptr<Word> ne;
        static const std::shared_ptr<Word> le;
        static const std::shared_ptr<Word> ge;
        static const std::shared_ptr<Word> minus;
        static const std::shared_ptr<Word> true_;
        static const std::shared_ptr<Word> false_;
        static const std::shared_ptr<Word> temp;
    };
}