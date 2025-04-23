#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include "Word.h"

namespace Lexer {
    class Lexer
    {
    public:
        static int line;



        char peek = ' ';

        std::unordered_map<std::string, Word> words;

        void reserve(const Word& w);

        Lexer();

        // 把下一个输入字符读到变量 peek 中
        void readch();

        // 识别复合的词法单元
        bool readch(char c);

        std::shared_ptr<Token> scan();

        // 字符转为十进制数值
        int digit(char c, int radix);
    };
}