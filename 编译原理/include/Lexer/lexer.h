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

        // ������Ϣ
        std::string error_info;

        DoubleBuffer<4096> buffer;

        char peek = ' ';

        std::unordered_map<std::string, std::shared_ptr<Word>> words;

        void reserve(std::shared_ptr<Word> w);

        Lexer(std::string filepath);

        Lexer& operator=(Lexer& lexer);

        // ����һ�������ַ��������� peek ��
        void readch();

        // ʶ�𸴺ϵĴʷ���Ԫ
        bool readch(char c);

        // ����״̬ö��
        enum class State : int {
            START = 0,
            IN_ID = 101,
            // �ַ���
            START_STRING = 201, 
            IN_NORMAL_STRING_CHAR = 202, // ��ͨ�ַ� [^\"]
            IN_ESCAPE_STATE = 203, // ����\��ʼת��
            IN_PARSE_OCT_1 = 204, // ����8���Ƶ�1������
            IN_PARSE_OCT_2 = 205, // ����8���Ƶ�2������
            IN_PARSE_OCT_3 = 206, // ����8���Ƶ�3������
            IN_PARSE_HEX_1 = 207, // ����16���Ƶ�1������
            IN_PARSE_HEX_n = 208, // ����16���Ƶ�n������
            END_STRING = 209, // �����ַ���
            ERROR_UNCLOSED_STRING = 210, //ȱ�ٹر������ַ���
            // ���ֲ���
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
            // �����ַ�
            START_CHAR = 401,
            IN_NORMAL_CHAR = 402,
            IN_ESCAPE_CHAR = 403,
            IN_PARSE_OCT_CHAR_1 = 404,
            IN_PARSE_OCT_CHAR_2 = 405,
            IN_PARSE_OCT_CHAR_3 = 406,
            IN_PARSE_HEX_CHAR_1 = 407,
            IN_PARSE_HEX_CHAR_n = 408,
            END_CHAR = 409,
            ERROR_UNCLOSED_CHAR = 410,
            ERROR_EMPTY_CHAR = 411,
            // ע��
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

        // �����ַ�����ö��
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
            DELIMITER, // ���޷�
            CHAR, // �����ַ�
            STAR, // [*]
            ZERO, // [0]
            NEW_LINE, // [\r:13]
            OTHER_CHAR, 
            EOF_CHAR, // [\0]
            LONG_SIGN,//long���͵Ľ����� [L]
            SCI_SIGN,//��ѧ���������� [eE]
            FLOAT_SIGN,//float���͵Ľ����� [f]
            HEX_SIGN,//ʮ�����Ƶ���ʼ���� [x]
            BIN_SIGN,//�����Ƶ���ʼ���� [b]
            SCI_SIGN_PULS_MINUS,//��ѧ�������� +��- ���� [+-]
            ESCAPABLE_CHAR, // ��ת���ַ� ["ntrvfab\']
            HEX_DIGIT,// 16�������� [0-9a-fA-F]
            NOT_HEX_DIGIT,// ��16�������� [^0-9a-fA-F]
            OCT_DIGIT,// 8�������� [0-7]
            NORMAL_STRING_CHAR, // �����ַ� [^\"EOF\r]
            NORMAL_CHAR, // �Ƿ�б�ܺ͵����� [^\']
            NOT_STRING_END_SIGN, // ���ַ����������� [^"EOF\r]
            NOT_ESCAPABLE_CHAR, // �ǿ�ת���ַ� [^"ntrvfab\']
        };

        std::unordered_map<State, std::unordered_map<char, CharType>> charTypeTable;

        // ״̬ת�Ʊ�
        std::unordered_map<State, std::unordered_map<CharType, State>> transitionTable = {
            {State::START, {
                {CharType::LETTER, State::IN_ID}, // 0 -> 101
                {CharType::DOUBLE_QOUTE, State::START_STRING}, // 0 -> 201
                {CharType::DIGIT_ONE, State::IN_NUM}, // 0 -> 301
                {CharType::ZERO, State::IN_OCT_NUM}, // 0 -> 303
                {CharType::DOT,State::IN_REAL}, // 0 -> 304
                {CharType::SINGLE_QOUTE, State::START_CHAR}, // 0 -> 401
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
            /*�����ַ���*/
            {State::START_STRING, { //201
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_STATE},//203
                {CharType::DOUBLE_QOUTE, State::END_STRING},//209
                {CharType::NORMAL_STRING_CHAR, State::IN_NORMAL_STRING_CHAR},//202
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_STRING},//210
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_STRING},//210
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_NORMAL_STRING_CHAR, { //202
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_STATE},//203
                {CharType::DOUBLE_QOUTE, State::END_STRING},//209
                {CharType::NORMAL_STRING_CHAR, State::IN_NORMAL_STRING_CHAR},//202
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_STRING},//210
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_STRING},//210
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_ESCAPE_STATE, {//203
                {CharType::ESCAPABLE_CHAR, State::IN_NORMAL_STRING_CHAR},//202
                {CharType::OCT_DIGIT, State::IN_PARSE_OCT_1},//204
                {CharType::HEX_SIGN, State::IN_PARSE_HEX_1},//207
                {CharType::NOT_ESCAPABLE_CHAR, State::START_STRING}, //201
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_STRING},//210
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_STRING},//210
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_PARSE_OCT_1, {//204
                {CharType::OCT_DIGIT, State::IN_PARSE_OCT_2},//205
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_STATE},//203
                {CharType::NORMAL_STRING_CHAR, State::IN_NORMAL_STRING_CHAR},//202
                {CharType::DOUBLE_QOUTE, State::END_STRING},//209
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_STRING},//210
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_STRING},//210
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_PARSE_OCT_2, {//205
                {CharType::OCT_DIGIT, State::IN_PARSE_OCT_3}, //206
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_STATE},//203
                {CharType::NORMAL_STRING_CHAR, State::IN_NORMAL_STRING_CHAR},//202
                {CharType::DOUBLE_QOUTE, State::END_STRING},//209
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_STRING},//210
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_STRING},//210
                {CharType::OTHER_CHAR, State::END},

            }},
            {State::IN_PARSE_OCT_3, {//206
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_STATE},//203
                {CharType::NORMAL_STRING_CHAR, State::IN_NORMAL_STRING_CHAR},//202
                {CharType::DOUBLE_QOUTE, State::END_STRING},//209
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_STRING},//210
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_STRING},//210
                {CharType::OTHER_CHAR, State::END},

            }},
            {State::IN_PARSE_HEX_1, {//207
                {CharType::NORMAL_STRING_CHAR, State::IN_NORMAL_STRING_CHAR},//202
                {CharType::HEX_DIGIT, State::IN_PARSE_HEX_n},//208
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_STRING},//210
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_STRING},//210
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_PARSE_HEX_n, {//208
                {CharType::HEX_DIGIT, State::IN_PARSE_HEX_n}, // 208
                {CharType::NORMAL_STRING_CHAR, State::IN_NORMAL_STRING_CHAR}, //202
                {CharType::DOUBLE_QOUTE, State::END_STRING}, //209
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_STRING},//210
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_STRING},//210
                {CharType::OTHER_CHAR, State::END},

            }},
            {State::END_STRING, {//209
                {CharType::EOF_CHAR, State::END},
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::ERROR_UNCLOSED_STRING, {//210
                {CharType::EOF_CHAR, State::END},
                {CharType::OTHER_CHAR, State::END},
            }},
            /*�����ַ�*/
            {State::START_CHAR, { // 401
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_CHAR}, //403
                {CharType::NORMAL_CHAR, State::IN_NORMAL_CHAR}, //402
                {CharType::SINGLE_QOUTE, State::ERROR_EMPTY_CHAR},//411
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_NORMAL_CHAR, { // 402
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_CHAR},//403
                {CharType::NORMAL_CHAR, State::IN_NORMAL_CHAR},//402
                {CharType::SINGLE_QOUTE, State::END_CHAR},//409
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_ESCAPE_CHAR, { // 403
                {CharType::ESCAPABLE_CHAR, State::IN_NORMAL_CHAR},//402
                {CharType::NOT_ESCAPABLE_CHAR, State::IN_NORMAL_CHAR},//402
                {CharType::HEX_SIGN, State::IN_PARSE_HEX_CHAR_1},
                {CharType::OCT_DIGIT, State::IN_PARSE_OCT_CHAR_1},
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_PARSE_OCT_CHAR_1, { // 404
                {CharType::OCT_DIGIT, State::IN_PARSE_OCT_CHAR_2},//405
                {CharType::SINGLE_QOUTE, State::END_CHAR}, //409
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_CHAR},//403
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_PARSE_OCT_CHAR_2, { // 405
                {CharType::OCT_DIGIT, State::IN_PARSE_OCT_CHAR_3},
                {CharType::SINGLE_QOUTE, State::END_CHAR}, //409
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_CHAR},//403
                {CharType::OTHER_CHAR, State::END},
            }},
            { State::IN_PARSE_OCT_CHAR_3, { // 406
                {CharType::SINGLE_QOUTE, State::END_CHAR}, //409
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_CHAR},//403
                {CharType::OTHER_CHAR, State::END},
            } },
            { State::IN_PARSE_HEX_CHAR_1, { // 407
                {CharType::NOT_HEX_DIGIT, State::IN_NORMAL_CHAR},//402
                {CharType::BACWARD_SLASH, State::IN_ESCAPE_CHAR},//403
                {CharType::HEX_DIGIT, State::IN_PARSE_HEX_CHAR_n},//408
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::OTHER_CHAR, State::END},
            } },
            { State::IN_PARSE_HEX_CHAR_n, { // 408
                {CharType::NOT_HEX_DIGIT, State::IN_NORMAL_CHAR},//402
                {CharType::SINGLE_QOUTE, State::END_CHAR},
                {CharType::HEX_DIGIT, State::IN_PARSE_HEX_CHAR_n},
                {CharType::NEW_LINE, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::EOF_CHAR, State::ERROR_UNCLOSED_CHAR},//410
                {CharType::NORMAL_CHAR, State::IN_NORMAL_CHAR}, //402
                {CharType::OTHER_CHAR, State::END},
            } },
            { State::END_CHAR, { // 409
                {CharType::EOF_CHAR, State::END},
                {CharType::OTHER_CHAR, State::END},
            } },
            { State::ERROR_UNCLOSED_CHAR, { // 410
                {CharType::EOF_CHAR, State::END},
                {CharType::OTHER_CHAR, State::END},
            } },
            { State::ERROR_EMPTY_CHAR, { // 411
                {CharType::EOF_CHAR, State::END},
                {CharType::OTHER_CHAR, State::END},
            } },
            /*����ע��*/
            {State::START_COMMENT, {
                {CharType::FORWARD_SLASH, State::IN_SINGLE_COMMENT},
                {CharType::STAR, State::IN_MUTI_COMMENT},
                {CharType::OPERATOR,State::END_OP},
                {CharType::OTHER_CHAR, State::END},
            }},
            {State::IN_SINGLE_COMMENT, {
                {CharType::CHAR, State::IN_SINGLE_COMMENT},
                {CharType::EOF_CHAR ,State::END},
                {CharType::NEW_LINE, State::END},
            }},
            {State::IN_MUTI_COMMENT, {
                {CharType::STAR, State::END_MUTI_COMMENT1}, // ����*���ܽ���ע��
                {CharType::CHAR, State::IN_MUTI_COMMENT}
            }},
            {State::END_MUTI_COMMENT1, {
                {CharType::FORWARD_SLASH, State::END}, // ����/����ע��
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
            //������ֲ��ֵ�״̬ת��
            {State::IN_NUM,{
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::IN_NUM},
                {CharType::LONG_SIGN,State::END_NUM_LONG},
                {CharType::SCI_SIGN,State::IN_SCI_NUM},
                {CharType::DOT,State::END_REAL},
                {CharType::EOF_CHAR, State::END},
            }},
            {State::END_NUM_LONG,{
                {CharType::OTHER_CHAR, State::END},
                {CharType::EOF_CHAR, State::END},
            }},
            {State::IN_REAL,{
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_REAL},
                {CharType::EOF_CHAR, State::END},
            }},
            {State::IN_OCT_NUM,{
                {CharType::OTHER_CHAR, State::END},
                {CharType::DOT,State::END_REAL},
                {CharType::DIGIT, State::IN_OCT_NUM},
                {CharType::HEX_SIGN, State::IN_HEX_NUM},
                {CharType::BIN_SIGN,State::IN_BIN_NUM},
                {CharType::EOF_CHAR, State::END},
            }},
            {State::END_REAL, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_REAL},
                {CharType::SCI_SIGN,State::IN_SCI_NUM},
                {CharType::FLOAT_SIGN,State::END_SCI_NUM_F},
                {CharType::EOF_CHAR, State::END},
            }},
            {State::IN_SCI_NUM, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::SCI_SIGN_PULS_MINUS,State::IN_SCI_SIGN},
                {CharType::DIGIT,State::END_SCI_NUM},
                {CharType::EOF_CHAR, State::END},
            }},
            { State::IN_SCI_SIGN, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_SCI_NUM},
                {CharType::EOF_CHAR, State::END},
            }},
            { State::END_SCI_NUM, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_SCI_NUM},
                {CharType::FLOAT_SIGN,State::END_SCI_NUM_F},
                {CharType::EOF_CHAR, State::END},
            }},
            { State::IN_HEX_NUM, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_HEX_NUM},
                {CharType::EOF_CHAR, State::END},
            } },
            { State::END_HEX_NUM, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_HEX_NUM},
                {CharType::EOF_CHAR, State::END},
            } },
            { State::IN_BIN_NUM, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_BIN_NUM},
                {CharType::EOF_CHAR, State::END},
            } }, 
            { State::END_BIN_NUM, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::DIGIT,State::END_BIN_NUM},
                {CharType::EOF_CHAR, State::END},
            } },
            { State::END_SCI_NUM_F, {
                {CharType::OTHER_CHAR, State::END},
                {CharType::EOF_CHAR, State::END},
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

        // �ַ�תΪʮ������ֵ
        int digit(char c, int radix);
    };
}