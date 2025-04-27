#include <fstream>
#include <unordered_map>
#include <string>
#include <chrono>
#include "Lexer/lexer.h"

using State = Lexer::Lexer::State;
using Type = Lexer::Lexer::CharType;

namespace Lexer {

    void Lexer::Lexer::generateAndSaveMap() {

        //初始状态
        transform(0, 'a', 'z', Type::LETTER);
        transform(0, 'A', 'Z', Type::LETTER);
        transform(0, '_', Type::LETTER);

        transform(0, "(){}[],;:.?", Type::DELIMITER);
        

        transform(0, "++--~!*/%+−<<>><><=>===!=&&||&|^=+=−=*=/=%=&=|=^=<<=>>=->->*.*:", Type::OPERATOR);
        transform(0, '/', Type::FORWARD_SLASH);
        transform(0, '\'', Type::SINGLE_QOUTE);
        transform(0, '"', Type::DOUBLE_QOUTE);
        transform(0, '1', '9', Type::DIGIT_ONE);
        transform(0, '.', Type::DOT);
        transform(0, '0', Type::ZERO);
        transform(0, " \t\r\v\f", Type::WHITESPACE);
        transform(0, '\n', Type::NEW_LINE);


        //101
        transform(101, 'a', 'z', Type::LETTER);
        transform(101, 'A', 'Z', Type::LETTER);
        transform(101, '_', Type::LETTER);
        transform(101, '0', '9', Type::DIGIT);
        //201
        transform(201, '\\', Type::BACWARD_SLASH);
        transform(201, '\"', Type::DOUBLE_QOUTE);
        transform(201, 0,'\"'-1, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(201, '\"'+1, '\\'-1, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(201, '\\'+1, 126, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        // 202
        transform(202, '\\', Type::BACWARD_SLASH);
        transform(202, '\"', Type::DOUBLE_QOUTE);
        transform(202, 0, '\"' - 1, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(202, '\"' + 1, '\\' - 1, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(202, '\\' + 1, 126, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        // 203
        transform(203, '\\', Type::ESCAPABLE_CHAR);
        transform(203, 'n', Type::ESCAPABLE_CHAR);
        transform(203, 't', Type::ESCAPABLE_CHAR);
        transform(203, 'r', Type::ESCAPABLE_CHAR);
        transform(203, 'v', Type::ESCAPABLE_CHAR);
        transform(203, 'f', Type::ESCAPABLE_CHAR);
        transform(203, 'a', Type::ESCAPABLE_CHAR);
        transform(203, 'b', Type::ESCAPABLE_CHAR);
        transform(203, 'e', Type::ESCAPABLE_CHAR);
        transform(203, '"', Type::ESCAPABLE_CHAR);
        transform(203, '\'', Type::ESCAPABLE_CHAR);
        transform(203, '0','7', Type::OCT_DIGIT);
        transform(203, 'x', Type::HEX_SIGN);
        //204
        transform(204, '\\', Type::BACWARD_SLASH);
        transform(204, 0, '\"' - 1, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(204, '\"' + 1, '\\' - 1, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(204, '\\' + 1, 126, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(204, '0', '7', Type::OCT_DIGIT);
        transform(204, '"', Type::DOUBLE_QOUTE);
        //205
        transform(205, '\\', Type::BACWARD_SLASH);
        transform(205, 0, '\"' - 1, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(205, '\"' + 1, '\\' - 1, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(205, '\\' + 1, 126, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(205, '0', '7', Type::OCT_DIGIT);
        transform(205, '"', Type::DOUBLE_QOUTE);
        //206
        transform(206, '\\', Type::BACWARD_SLASH);
        transform(206, 0, '\"' - 1, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(206, '\"' + 1, '\\' - 1, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(206, '\\' + 1, 126, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(206, '"', Type::DOUBLE_QOUTE);
        //207
        transform(207, '0', '9', Type::HEX_DIGIT);
        transform(207, 'a', 'f', Type::HEX_DIGIT);
        transform(207, 'A', 'F', Type::HEX_DIGIT);
        //208
        
        transform(208, 0, '\"' - 1, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(208, '\"' + 1, '\\' - 1, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);
        transform(208, '\\' + 1, 126, Type::NOT_BACKWARD_SLASH_OR_DOUBLE_QOUTE);

        transform(208, '0', '9', Type::HEX_DIGIT);
        transform(208, 'a', 'f', Type::HEX_DIGIT);
        transform(208, 'A', 'F', Type::HEX_DIGIT);
        transform(208, '"', Type::DOUBLE_QOUTE);
        //301
        transform(301, '0', '9', Type::DIGIT);
        transform(301, "Ll", Type::LONG_SIGN);
        transform(301, "Ee", Type::SCI_SIGN);
        transform(301, '.', Type::DOT);

        //303
        transform(303, '0', '7', Type::DIGIT);
        transform(303, 'f', Type::FLOAT_SIGN);
        transform(303, 'F', Type::FLOAT_SIGN);
        transform(303, '.', Type::DOT);
        transform(303, 'e', Type::SCI_SIGN);
        transform(303, 'E', Type::SCI_SIGN);
        transform(303, 'x', Type::HEX_SIGN);
        transform(303, 'X', Type::HEX_SIGN);
        transform(303, 'b', Type::BIN_SIGN);
        transform(303, 'B', Type::BIN_SIGN);
        //304
        transform(304, '0', '9', Type::DIGIT);

        //305
        transform(305, '0', '9', Type::DIGIT);
        transform(305, 'e', Type::SCI_SIGN);
        transform(305, 'E', Type::SCI_SIGN);
        transform(305, 'f', Type::FLOAT_SIGN);
        transform(305, 'F', Type::FLOAT_SIGN);
        //306
        transform(306, '0', '9', Type::DIGIT);
        transform(306, '+', Type::SCI_SIGN_PULS_MINUS);
        transform(306, '-', Type::SCI_SIGN_PULS_MINUS);
        //307
        transform(307, '0', '9', Type::DIGIT);
        //308
        transform(308, '0', '9', Type::DIGIT);
        transform(308, 'f', Type::FLOAT_SIGN);
        transform(308, 'F', Type::FLOAT_SIGN);
        //309
        transform(309, '0', '1', Type::DIGIT);
        //310
        transform(310, '0', '9', Type::DIGIT);
        transform(310, 'a', 'f', Type::DIGIT);
        transform(310, 'A', 'F', Type::DIGIT);
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
        transform(504, 0, 126, Type::CHAR);
        transform(504, '*', Type::STAR);
        transform(504, '/', Type::FORWARD_SLASH);
        //601
        transform(601, "++--~!*/%+−<<>><><=>===!=&&||&|^=+=−=*=/=%=&=|=^=<<=>>=->->*.*:", Type::OPERATOR);
    }
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