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

        // 错误信息
        std::string error_info;

        DoubleBuffer<4096> buffer;

        char peek = ' ';

        std::unordered_map<std::string, Word> words;

        void reserve(const Word& w);

        Lexer(std::string filepath);

        Lexer& operator=(Lexer& lexer);

        // 把下一个输入字符读到变量 peek 中
        void readch();

        // 识别复合的词法单元
        bool readch(char c);

        // 定义状态枚举
        enum class State : int {
            START = 0,
            IN_ID = 101,
            // 字符串
            START_STRING = 201, 
            IN_NORMAL_STRING_CHAR = 202, // 普通字符 [^\"]
            IN_ESCAPE_STATE = 203, // 接收\开始转义
            IN_PARSE_OCT_1 = 204, // 解析8进制第1个数字
            IN_PARSE_OCT_2 = 205, // 解析8进制第2个数字
            IN_PARSE_OCT_3 = 206, // 解析8进制第3个数字
            IN_PARSE_HEX_1 = 207, // 解析16进制第1个数字
            IN_PARSE_HEX_n = 208, // 解析16进制第n个数字
            END_STRING = 209, // 结束字符串
            // 数字部分
            IN_NUM = 301,
            END_NUM_LONG = 302,
            IN_OCT_NUM = 303,
            IN_REAL = 304,
            END_REAL = 305,
            IN_SCI_NUM = 306,
            IN_SCI_SIGN = 307,
            END_SCI_NUM = 308,
            IN_BIN_NUM = 309,
            IN_HEX_NUM = 310,
            END_SCI_NUM_F = 311,
            END_BIN_NUM = 312,
            END_HEX_NUM = 313,
            // 单个字符
            IN_CHAR = 401,
            IN_NORMAL_CHAR = 402,
            IN_ESCAPE_CHAR = 403,
            IN_PARSE_OCT_CHAR_1 = 404,
            IN_PARSE_OCT_CHAR_2 = 405,
            IN_PARSE_OCT_CHAR_3 = 406,
            IN_PARSE_HEX_CHAR_1 = 407,
            IN_PARSE_HEX_CHAR_n = 408,
            IN_PARSE_ESCAPABLE_CHAR = 409,
            END_CHAR = 410,
            // 注释
            START_COMMENT = 501,
            IN_SINGLE_COMMENT = 502,
            IN_MUTI_COMMENT = 503,
            END_MUTI_COMMENT1 = 504,
            END_MUTI_COMMENT2 = 505,
            END_SINGLE_COMMENT = 506,
            IN_OP = 601,
            END_OP = 602,
            END_DELIMITER = 701,
            END=999
        };

        // 定义字符类型枚举
        enum class CharType : int {
            LETTER, //[a-zA-Z_]
            DIGIT, // [0-9]
            DIGIT_ONE, // [1-9]
            DOT, // [.]
            WHITESPACE, // 
            OPERATOR, // 
            SINGLE_QOUTE, // [']
            DOUBLE_QOUTE, // ["]
            FORWARD_SLASH, // [/]
            BACWARD_SLASH, // [\]
            DELIMITER, // 界限符
            CHAR, // 所有字符
            STAR, // [*]
            ZERO, // [0]
            NEW_LINE, // [\n]
            OTHER_CHAR, 
            EOF_CHAR, // [\0]
            LONG_SIGN,//long类型的结束符 [L]
            SCI_SIGN,//科学计数法符号 [eE]
            FLOAT_SIGN,//float类型的结束符 [f]
            HEX_SIGN,//十六进制的起始符号 [x]
            BIN_SIGN,//二进制的起始符号 [b]
            SCI_SIGN_PULS_MINUS,//科学技术法的 +、- 符号 [+-]
            ESCAPABLE_CHAR, // 可转义字符 ["ntrvfab\']
            HEX_DIGIT,// 16进制数字 [0-9a-fA-F]
            OCT_DIGIT,// 8进制数字 [0-7]
            NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE, // 非反斜杠和双引号 [^\"]
            NOT_BACKWARD_SLASH_OR_SINGLE_QOUTE, // 非反斜杠和单引号 [^\']
        };

        std::unordered_map<State, std::unordered_map<char, CharType>> charTypeTable;

        // 状态转移表
        std::unordered_map<State, std::unordered_map<CharType, State>> transitionTable = {
            {State::START, {
                {CharType::LETTER, State::IN_ID}, // 0 -> 101
                {CharType::DOUBLE_QOUTE, State::START_STRING}, // 0 -> 201
                {CharType::DIGIT_ONE, State::IN_NUM}, // 0 -> 301
                {CharType::ZERO, State::IN_OCT_NUM}, // 0 -> 303
                {CharType::DOT,State::IN_REAL}, // 0 -> 304
                {CharType::SINGLE_QOUTE, State::IN_CHAR}, // 0 -> 401
                {CharType::FORWARD_SLASH, State::START_COMMENT}, // 0 -> 501
                {CharType::OPERATOR, State::IN_OP}, // 0 -> 601
                {CharType::DELIMITER, State::END_DELIMITER}, // 0 -> 701
                {CharType::WHITESPACE, State::START}, // 0 -> 0
                {CharType::NEW_LINE, State::START},
                {CharType::OTHER_CHAR, State::END},
                {CharType::EOF_CHAR, State::END}
            }},
            {State::IN_ID, {
                {CharType::LETTER, State::IN_ID},
                {CharType::DIGIT, State::IN_ID},
                {CharType::OTHER_CHAR, State::END},
                {CharType::WHITESPACE, State::END},
                {CharType::DELIMITER, State::END},
                {CharType::OPERATOR, State::END},
                {CharType::EOF_CHAR, State::END}
            }},
            /*解析字符串*/
            {State::START_STRING, { //201
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_STATE},
                {CharType::DOUBLE_QOUTE, State::END_STRING},
                {CharType::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE, State::IN_NORMAL_STRING_CHAR},
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_NORMAL_STRING_CHAR, { //202
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_STATE},
                {CharType::DOUBLE_QOUTE, State::END_STRING},
                {CharType::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE, State::IN_NORMAL_STRING_CHAR},
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_ESCAPE_STATE, {//203
                {CharType::ESCAPABLE_CHAR, State::START_STRING},
                {CharType::OCT_DIGIT, State::IN_PARSE_OCT_1},
                {CharType::HEX_SIGN, State::IN_PARSE_HEX_1},
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_PARSE_OCT_1, {//204
                {CharType::OCT_DIGIT, State::IN_PARSE_OCT_2},
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_STATE},
                {CharType::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE, State::IN_NORMAL_STRING_CHAR},
                {CharType::DOUBLE_QOUTE, State::END_STRING},
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_PARSE_OCT_2, {//205
                {CharType::OCT_DIGIT, State::IN_PARSE_OCT_3},
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_STATE},
                {CharType::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE, State::IN_NORMAL_STRING_CHAR},
                {CharType::DOUBLE_QOUTE, State::END_STRING},
                {CharType::OTHER_CHAR, State::END},

            }},
            {State::IN_PARSE_OCT_3, {//206
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_STATE},
                {CharType::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE, State::IN_NORMAL_STRING_CHAR},
                {CharType::DOUBLE_QOUTE, State::END_STRING},
                {CharType::OTHER_CHAR, State::END},

            }},
            {State::IN_PARSE_HEX_1, {//207
                {CharType::HEX_DIGIT, State::IN_PARSE_HEX_n},
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_PARSE_HEX_n, {//208
                {CharType::HEX_DIGIT, State::IN_PARSE_HEX_n},
                {CharType::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE, State::IN_NORMAL_STRING_CHAR},
                {CharType::DOUBLE_QOUTE, State::END_STRING},
                {CharType::OTHER_CHAR, State::END},

            }},
            {State::END_STRING, {//209
                {CharType::EOF_CHAR, State::END},
                {CharType::OTHER_CHAR, State::END},
            }},
            /*解析字符*/
            {State::IN_CHAR, { // 401
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_CHAR},
                {CharType::NOT_BACKWARD_SLASH_OR_SINGLE_QOUTE, State::IN_NORMAL_CHAR},
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_NORMAL_CHAR, { // 402
                {CharType::SINGLE_QOUTE, State::END_CHAR},
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_ESCAPE_CHAR, { // 403
                {CharType::ESCAPABLE_CHAR, State::IN_PARSE_ESCAPABLE_CHAR},
                {CharType::HEX_SIGN, State::IN_PARSE_HEX_CHAR_1},
                {CharType::OCT_DIGIT, State::IN_PARSE_OCT_CHAR_1},
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_PARSE_OCT_CHAR_1, { // 404
                {CharType::OCT_DIGIT, State::IN_PARSE_OCT_CHAR_2},
                {CharType::SINGLE_QOUTE, State::END},
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_PARSE_OCT_CHAR_2, { // 405
                {CharType::OCT_DIGIT, State::IN_PARSE_OCT_CHAR_3},
                {CharType::SINGLE_QOUTE, State::END},
                {CharType::OTHER_CHAR, State::END},
            }},
            { State::IN_PARSE_OCT_CHAR_3, { // 406
                {CharType::SINGLE_QOUTE, State::END_CHAR},
                {CharType::OTHER_CHAR, State::END},
            } },
            { State::IN_PARSE_HEX_CHAR_1, { // 407
                {CharType::HEX_DIGIT, State::IN_PARSE_HEX_CHAR_n},
                {CharType::OTHER_CHAR, State::END},
            } },
            { State::IN_PARSE_HEX_CHAR_n, { // 408
                {CharType::SINGLE_QOUTE, State::END_CHAR},
                {CharType::HEX_DIGIT, State::IN_PARSE_HEX_CHAR_n},
                {CharType::OTHER_CHAR, State::END},
            } },
            { State::IN_PARSE_ESCAPABLE_CHAR, { // 409
                {CharType::SINGLE_QOUTE, State::END_CHAR},
                {CharType::OTHER_CHAR, State::END},
            } },
            {State::END_CHAR, { // 410
                {CharType::EOF_CHAR, State::END},
                {CharType::OTHER_CHAR, State::END},
            }},
            /*解析注释*/
            {State::START_COMMENT, {
                {CharType::FORWARD_SLASH, State::IN_SINGLE_COMMENT},
                {CharType::STAR, State::IN_MUTI_COMMENT},
                {CharType::OPERATOR,State::END_OP},
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_SINGLE_COMMENT, {
                {CharType::CHAR, State::IN_SINGLE_COMMENT},
                {CharType::NEW_LINE, State::END},
            }},
            {State::IN_MUTI_COMMENT, {
                {CharType::STAR, State::END_MUTI_COMMENT1}, // 遇到*可能结束注释
                {CharType::CHAR, State::IN_MUTI_COMMENT}
            }},
            {State::END_MUTI_COMMENT1, {
                {CharType::FORWARD_SLASH, State::END}, // 遇到/结束注释
                {CharType::CHAR, State::IN_MUTI_COMMENT},
                {CharType::STAR, State::END_MUTI_COMMENT1},
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
            //添加数字部分的状态转移
            {State::IN_NUM,{
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::IN_NUM},
                {CharType::LONG_SIGN,State::END_NUM_LONG},
                {CharType::SCI_SIGN,State::IN_SCI_NUM},
                {CharType::DOT,State::END_REAL},
            }},
            {State::END_NUM_LONG,{
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_REAL,{
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_REAL},
            }},
            {State::IN_OCT_NUM,{
                {CharType::OTHER_CHAR, State::END},
                {CharType::DOT,State::END_REAL},
                {CharType::DIGIT, State::IN_OCT_NUM},
                {CharType::HEX_SIGN, State::IN_HEX_NUM},
                {CharType::BIN_SIGN,State::IN_BIN_NUM},
            }},
            {State::END_REAL, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_REAL},
                {CharType::SCI_SIGN,State::IN_SCI_NUM},
                {CharType::FLOAT_SIGN,State::END_SCI_NUM_F},
            }},
            {State::IN_SCI_NUM, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::SCI_SIGN_PULS_MINUS,State::IN_SCI_SIGN},
                {CharType::DIGIT,State::END_SCI_NUM},
            }},
            { State::IN_SCI_SIGN, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_SCI_NUM},
            }},
            { State::END_SCI_NUM, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_SCI_NUM},
                {CharType::FLOAT_SIGN,State::END_SCI_NUM_F},
            }},
            { State::IN_HEX_NUM, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_HEX_NUM},
            } },
            { State::END_HEX_NUM, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_HEX_NUM},
            } },
            { State::IN_BIN_NUM, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_BIN_NUM},
            } }, 
            { State::END_BIN_NUM, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_BIN_NUM},
            } },
            { State::END_SCI_NUM_F, {
                {CharType::OTHER_CHAR, State::END},
            } },
        };

        CharType getCharType(char c, State currentState);

        void loadTableFromFile(const std::string& filename);

        std::shared_ptr<Word> getOperationToken(const std::string& op);

        void generateAndSaveMap();
        void transform(int state, std::string str, CharType type);
        void transform(int state, char c, CharType type);
        void transform(int state, char start, char end, CharType type);
        std::shared_ptr<Token> scan();

        // 字符转为十进制数值
        int digit(char c, int radix);
    };
}