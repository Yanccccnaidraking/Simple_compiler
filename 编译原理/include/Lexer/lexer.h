#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include "Word.h"
#include "doublebuffer.h"

namespace Lexer {
    class Lexer
    {
    public:
        static int line;

        DoubleBuffer<4096> buffer;

        char peek = ' ';

        std::unordered_map<std::string, Word> words;

        void reserve(const Word& w);

        Lexer(std::string filepath);

        // ����һ�������ַ��������� peek ��
        void readch();

        // ʶ�𸴺ϵĴʷ���Ԫ
        bool readch(char c);

        std::shared_ptr<Token> scan();

        // �ַ�תΪʮ������ֵ
        int digit(char c, int radix);
    };
}