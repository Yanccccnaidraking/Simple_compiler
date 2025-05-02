#pragma once
#include "Token.h"
#include "Tag.h"
#include <string>

namespace Lexer {
    // �� Word ���ڹ������֡���ʶ������ && �����ĸ��ϴʷ���Ԫ�Ĵ���
    // Ҳ�����������м���������������д��ʽ
    class Word : public Token
    {
    public:
        std::string lexeme = "";

        Word() :lexeme(""), Token() {}
        Word(const std::string& s, int tag) : lexeme(s), Token(tag) {}
        Word(const std::string& s, Tag tag) : lexeme(s), Token(tag) {}

        //// ��ʽ���������Ա����
        //Word(const Word&) = default;
        //Word& operator=(const Word&) = delete;  // ���ÿ�����ֵ
        //Word(Word&&) = default;                // �����ƶ�����
        //Word& operator=(Word&&) = delete;      // �����ƶ���ֵ


        std::string toString() const { return lexeme; }

        static const Word and_;
        static const Word or_;
        static const Word eq;
        static const Word ne;
        static const Word le;
        static const Word ge;
        static const Word minus;
        static const Word true_;
        static const Word false_;
        static const shared_ptr<Word> temp;
    };
}