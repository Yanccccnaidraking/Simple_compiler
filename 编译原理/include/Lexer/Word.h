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