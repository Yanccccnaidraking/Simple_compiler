#include <fstream>
#include <unordered_map>
#include <string>
#include <chrono>
#include "Lexer/lexer.h"

using State = Lexer::Lexer::State;
using Type = Lexer::Lexer::CharType;


std::unordered_map<Lexer::Lexer::State, std::unordered_map<char, Lexer::Lexer::CharType>> table;

void transform(int state, char start, char end, Type type)
{
    for (char i = start; i <= end; i++)
    {
        table[(State)state][i] = type;
    }
}

void transform(int state, char c, Type type)
{
    table[(State)state][c] = type;
}

void transform(int state, std::string str, Type type)
{
    for (auto c : str)
    {
        table[(State)state][c] = type;
    }
}

void generateAndSaveMap() {

    //初始状态
    transform(0, 'a', 'z', Type::LETTER);
    transform(0, '_', Type::LETTER);

    transform(0, "(){}[],;:.?", Type::DELIMITER);
    transform(0, '/', Type::FORWARD_SLASH);
    
    transform(0, "++--~!*/%+−<<>><><=>===!=&&||&|^=+=−=*=/=%=&=|=^=<<=>>=->->*.*:", Type::OPERATOR);
    transform(0, '\'', Type::SINGLE_QOUTE);
    transform(0, '"', Type::DOUBLE_QOUTE);
    transform(0, '1', '9', Type::DIGIT_ONE);
    transform(0, '.', Type::DOT);
    transform(0, '0', Type::ZERO);
    transform(0, " \t\r\v\f", Type::WHITESPACE);
    transform(0, '\n', Type::NEW_LINE);


    //101
    transform(101, 'a', 'z', Type::LETTER);
    transform(101, '_', Type::LETTER);
    transform(101, '0', '9', Type::DIGIT);
    //201
    transform(201, 'a','z', Type::LETTER);
    transform(201, '"', Type::DOUBLE_QOUTE);
    //301
    transform(301, '0', '9', Type::DIGIT);
    transform(301, 'l', Type::LONG_SIGN);
    transform(301, 'e', Type::SCI_SIGN);
    transform(301, '.', Type::DOT);

    //303
    transform(303, '0', '7', Type::DIGIT);
    transform(303, 'f', Type::FLOAT_SIGN);
    transform(303, '.', Type::DOT);
    transform(303, 'e', Type::SCI_SIGN);
    transform(303, 'x', Type::HEX_SIGN);
    transform(303, 'b', Type::BIN_SIGN);
    //304
    transform(304, '0', '9', Type::DIGIT);

    //305
    transform(305, '0', '9', Type::DIGIT);
    transform(305, 'e', Type::SCI_SIGN);
    transform(305, 'f', Type::FLOAT_SIGN);
    //306
    transform(306, '0', '9', Type::DIGIT);
    transform(306, '+',Type::SCI_SIGN_PULS_MINUS );
    transform(306, '-', Type::SCI_SIGN_PULS_MINUS);
    //307
    transform(307, '0', '9', Type::DIGIT);
    //308
    transform(308, '0', '9', Type::DIGIT);
    transform(308, 'f', Type::FLOAT_SIGN);
    //309
    transform(309, '0', '1', Type::DIGIT);
    //310
    transform(310, '0', '9', Type::DIGIT);
    transform(310, 'a', 'f', Type::DIGIT);
    //401
    transform(401, 0, 126, Type::CHAR);
    //402
    transform(402, '\'', Type::SINGLE_QOUTE);
    //501
    transform(501, '*', Type::STAR);
    transform(501, '/', Type::FORWARD_SLASH);
    //502
    transform(502, 0, 126, Type::CHAR);
    transform(502, '\n', Type::NEW_LINE);
    //503
    transform(503, 0, 126, Type::CHAR);
    transform(503, '*', Type::STAR);
    //504
    transform(504, '/', Type::FORWARD_SLASH);
    //601
    transform(601, "++--~!*/%+−<<>><><=>===!=&&||&|^=+=−=*=/=%=&=|=^=<<=>>=->->*.*:", Type::OPERATOR);
}



void saveTableToFile(const std::string& filename) {
    std::ofstream out(filename);
    for (const auto& statePair : table) {
        auto state = static_cast<int>(statePair.first);
        for (const auto& charPair : statePair.second) {
            char ch = charPair.first;
            auto type = static_cast<int>(charPair.second);
            out << state << " " << static_cast<int>(ch) << " " << type << "\n";
        }
    }
    out.close();
}

/// <summary>
/// 运行前把main.cpp注释掉
/// </summary>
/// <returns></returns>
//int main()
//{
//    // 记录开始时间
//    auto start = std::chrono::high_resolution_clock::now();
//    generateAndSaveMap();
//    /*saveTableToFile("CharTypeTable.txt");*/
//    // 记录结束时间
//    auto end = std::chrono::high_resolution_clock::now();
//
//    // 计算耗时（单位：毫秒）
//    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//
//    std::cout << "耗时：" << duration.count() << " 毫秒" << std::endl;
//    return 0;
//}