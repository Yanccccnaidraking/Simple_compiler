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

        // ����һ�������ַ��������� peek ��
        void readch();

        // ʶ�𸴺ϵĴʷ���Ԫ
        bool readch(char c);

        std::shared_ptr<Token> scan();

        // �ַ�תΪʮ������ֵ
        int digit(char c, int radix);
    };
}