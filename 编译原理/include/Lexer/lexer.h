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

        // 把下一个输入字符读到变量 peek 中
        void readch();

        // 识别复合的词法单元
        bool readch(char c);

        // 定义状态枚举
        enum class State : int {
            START = 0,
            IN_ID = 101,
            IN_STRING = 201,
            END_STRING = 202,
            // 数字部分
            //IN_NUM = 302,
            //END_NUM = 312,    
            //IN_OCT_NUM = 303,
            //IN_REAL = 304,
            //END_REAL = 305,
            //IN_SCI_NUM = 306,
            //IN_SCI_SIGN = 307, 
            //END_SCI_NUM = 308,
            //END_SCI_NUM_F = 311,
            //IN_BIN_NUM = 309,
            //IN_HEX_NUM = 310,
            START_CHAR = 401,
            IN_CHAR = 402,
            END_CHAR = 403,
            START_COMMENT = 501,
            IN_SINGLE_COMMENT = 502,
            IN_MUTI_COMMENT = 503,
            END_MUTI_COMMENT1 = 504,
            END_MUTI_COMMENT2 = 505,
            END_SINGLE_COMMENT = 506,
            IN_OP = 601,
            END_OP = 602,
            END_DELIMITER = 701,
            END
        };

        // 定义字符类型枚举
        enum class CharType : int {
            LETTER,
            DIGIT,
            DOT,
            WHITESPACE,
            OPERATOR,
            SINGLE_QOUTE,
            DOUBLE_QOUTE,
            FORWARD_SLASH,
            BACWARD_SLASH,
            DELIMITER,
            CHAR,
            STAR,
            ZERO,
            NEW_LINE,
            OTHER_CHAR,
            EOF_CHAR
        };

        // 状态转移表
        std::unordered_map<State, std::unordered_map<CharType, State>> transitionTable = {
            {State::START, {
                {CharType::LETTER, State::IN_ID}, // 0 -> 101
                {CharType::DOUBLE_QOUTE, State::IN_STRING}, // 0 -> 201
                {CharType::SINGLE_QOUTE, State::START_CHAR}, // 0 -> 401
                {CharType::FORWARD_SLASH, State::START_COMMENT}, // 0 -> 501
                {CharType::OPERATOR, State::IN_OP}, // 0 -> 601
                {CharType::DELIMITER, State::END_DELIMITER}, // 0 -> 701
                {CharType::WHITESPACE, State::START}, // 0 -> 0
                {CharType::NEW_LINE, State::START},
                {CharType::EOF_CHAR, State::END}
            }},
            {State::IN_ID, {
                {CharType::LETTER, State::IN_ID},
                {CharType::DIGIT, State::IN_ID},
                {CharType::WHITESPACE, State::END},
                {CharType::DELIMITER, State::END},
                {CharType::OPERATOR, State::END},
                {CharType::EOF_CHAR, State::END}
            }},
            {State::IN_STRING, {
                {CharType::CHAR, State::IN_STRING},
                {CharType::DOUBLE_QOUTE, State::END_STRING},
            }},
            {State::END_STRING, {
                {CharType::DOUBLE_QOUTE, State::END},
            }},
            {State::START_CHAR, {
                {CharType::CHAR, State::IN_CHAR}
            }},
            {State::IN_CHAR, {
                {CharType::SINGLE_QOUTE, State::END}
            }},
            {State::START_COMMENT, {
                {CharType::FORWARD_SLASH, State::IN_SINGLE_COMMENT},
                {CharType::STAR, State::IN_MUTI_COMMENT}
            }},
            {State::IN_SINGLE_COMMENT, {
                {CharType::CHAR, State::IN_SINGLE_COMMENT},
                {CharType::NEW_LINE, State::END}
            }},
            {State::IN_MUTI_COMMENT, {
                {CharType::STAR, State::END_MUTI_COMMENT1}, // 遇到*可能结束注释
                {CharType::OTHER_CHAR, State::IN_MUTI_COMMENT}
            }},
            {State::END_MUTI_COMMENT1, {
                {CharType::FORWARD_SLASH, State::END}, // 遇到/结束注释
                {CharType::OTHER_CHAR, State::IN_MUTI_COMMENT}
            }},
            {State::IN_OP, {
                {CharType::OPERATOR, State::END_OP},
                {CharType::OTHER_CHAR, State::END},
                {CharType::WHITESPACE, State::END},
            }},
            {State::END_OP, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::WHITESPACE, State::END},
            }},
            {State::END_DELIMITER, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::EOF_CHAR, State::END}
            }},
        };

        CharType getCharType(char c, State currentState);

        std::shared_ptr<Token> scan();

        // 字符转为十进制数值
        int digit(char c, int radix);
    };
}