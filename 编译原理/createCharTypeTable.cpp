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
        transform(201, -128,'\"'-1, Type::NORMAL_STRING_CHAR);
        transform(201, '\"'+1, '\\'-1, Type::NORMAL_STRING_CHAR);
        transform(201, '\\'+1, 126, Type::NORMAL_STRING_CHAR);
        transform(201, '\n', Type::NEW_LINE);
        transform(201, '\r', Type::NEW_LINE);
        transform(201, EOF, Type::EOF_CHAR);
        // 202
        transform(202, '\\', Type::BACWARD_SLASH);
        transform(202, '\"', Type::DOUBLE_QOUTE);
        transform(202, -128, '\"' - 1, Type::NORMAL_STRING_CHAR);
        transform(202, '\"' + 1, '\\' - 1, Type::NORMAL_STRING_CHAR);
        transform(202, '\\' + 1, 126, Type::NORMAL_STRING_CHAR);
        transform(202, '\n', Type::NEW_LINE);
        transform(202, '\r', Type::NEW_LINE);
        transform(202, EOF, Type::EOF_CHAR);
        // 203
        transform(203, 0, 126, Type::NOT_ESCAPABLE_CHAR);
        transform(203, '\\', Type::ESCAPABLE_CHAR);
        transform(203, 'n', Type::ESCAPABLE_CHAR);
        transform(203, 't', Type::ESCAPABLE_CHAR);
        transform(203, 'r', Type::ESCAPABLE_CHAR);
        transform(203, 'v', Type::ESCAPABLE_CHAR);
        transform(203, 'f', Type::ESCAPABLE_CHAR);
        transform(203, 'a', Type::ESCAPABLE_CHAR);
        transform(203, 'b', Type::ESCAPABLE_CHAR);
        transform(203, '"', Type::ESCAPABLE_CHAR);
        transform(203, '\'', Type::ESCAPABLE_CHAR);
        transform(203, '0','7', Type::OCT_DIGIT);
        transform(203, 'x', Type::HEX_SIGN);
        transform(203, '\n', Type::NEW_LINE);
        transform(203, '\r', Type::NEW_LINE);
        transform(203, EOF, Type::EOF_CHAR);
        //204
        transform(204, '\\', Type::BACWARD_SLASH);
        transform(204, -128, '\"' - 1, Type::NORMAL_STRING_CHAR);
        transform(204, '\"' + 1, '\\' - 1, Type::NORMAL_STRING_CHAR);
        transform(204, '\\' + 1, 126, Type::NORMAL_STRING_CHAR);
        transform(204, '0', '7', Type::OCT_DIGIT);
        transform(204, '"', Type::DOUBLE_QOUTE);
        transform(204, '\n', Type::NEW_LINE);
        transform(204, '\r', Type::NEW_LINE);
        transform(204, EOF, Type::EOF_CHAR);
        //205
        transform(205, '\\', Type::BACWARD_SLASH);
        transform(205, -128, '\"' - 1, Type::NORMAL_STRING_CHAR);
        transform(205, '\"' + 1, '\\' - 1, Type::NORMAL_STRING_CHAR);
        transform(205, '\\' + 1, 126, Type::NORMAL_STRING_CHAR);
        transform(205, '0', '7', Type::OCT_DIGIT);
        transform(205, '"', Type::DOUBLE_QOUTE);
        transform(205, '\n', Type::NEW_LINE);
        transform(205, '\r', Type::NEW_LINE);
        transform(205, EOF, Type::EOF_CHAR);
        //206
        transform(206, '\\', Type::BACWARD_SLASH);
        transform(206, -128, '\"' - 1, Type::NORMAL_STRING_CHAR);
        transform(206, '\"' + 1, '\\' - 1, Type::NORMAL_STRING_CHAR);
        transform(206, '\\' + 1, 126, Type::NORMAL_STRING_CHAR);
        transform(206, '"', Type::DOUBLE_QOUTE);
        transform(206, '\n', Type::NEW_LINE);
        transform(206, '\r', Type::NEW_LINE);
        transform(206, EOF, Type::EOF_CHAR);
        //207
        transform(207, '0', '9', Type::HEX_DIGIT);
        transform(207, 'a', 'f', Type::HEX_DIGIT);
        transform(207, 'A', 'F', Type::HEX_DIGIT);
        transform(207, '\n', Type::NEW_LINE);
        transform(207, '\r', Type::NEW_LINE);
        transform(207, EOF, Type::EOF_CHAR);
        //208
        
        transform(208, -128, '\"' - 1, Type::NORMAL_STRING_CHAR);
        transform(208, '\"' + 1, '\\' - 1, Type::NORMAL_STRING_CHAR);
        transform(208, '\\' + 1, 126, Type::NORMAL_STRING_CHAR);


        transform(208, '0', '9', Type::HEX_DIGIT);
        transform(208, 'a', 'f', Type::HEX_DIGIT);
        transform(208, 'A', 'F', Type::HEX_DIGIT);
        transform(208, '"', Type::DOUBLE_QOUTE);

        transform(208, '\n', Type::NEW_LINE);
        transform(208, '\r', Type::NEW_LINE);
        transform(208, EOF, Type::EOF_CHAR);

        //209
        
        //210


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
        //312
        transform(312, '0', '1', Type::DIGIT);
        //313
        transform(313, '0', '9', Type::DIGIT);
        transform(313, 'a', 'f', Type::DIGIT);
        transform(313, 'A', 'F', Type::DIGIT);
        //401
        transform(401, '\\', Type::BACWARD_SLASH);
        transform(401, 0, '\''-1, Type::NORMAL_CHAR);
        transform(401, '\''+1,'\\'-1, Type::NORMAL_CHAR);
        transform(401, '\\'+1, 126, Type::NORMAL_CHAR);
        transform(401, '\n', Type::NEW_LINE);
        transform(401, '\r', Type::NEW_LINE);
        transform(401, EOF, Type::EOF_CHAR);
        //402
        transform(402, '\\', Type::BACWARD_SLASH);
        transform(402, 0, '\'' - 1, Type::NORMAL_CHAR);
        transform(402, '\'' + 1, '\\' - 1, Type::NORMAL_CHAR);
        transform(402, '\\' + 1, 126, Type::NORMAL_CHAR);
        transform(402, '\n', Type::NEW_LINE);
        transform(402, '\r', Type::NEW_LINE);
        transform(402, '\'', Type::SINGLE_QOUTE);
        transform(402, EOF, Type::EOF_CHAR);
        //403
        transform(403, 0, 126, Type::NOT_ESCAPABLE_CHAR);
        transform(403, '\\', Type::ESCAPABLE_CHAR);
        transform(403, 'n', Type::ESCAPABLE_CHAR);
        transform(403, 't', Type::ESCAPABLE_CHAR);
        transform(403, 'r', Type::ESCAPABLE_CHAR);
        transform(403, 'v', Type::ESCAPABLE_CHAR);
        transform(403, 'f', Type::ESCAPABLE_CHAR);
        transform(403, 'a', Type::ESCAPABLE_CHAR);
        transform(403, 'b', Type::ESCAPABLE_CHAR);
        transform(403, '"', Type::ESCAPABLE_CHAR);
        transform(403, '\'', Type::ESCAPABLE_CHAR);
        transform(403, '0', '7', Type::OCT_DIGIT);
        transform(403, 'x', Type::HEX_SIGN);
        transform(403, '\n', Type::NEW_LINE);
        transform(403, '\r', Type::NEW_LINE);
        transform(403, EOF, Type::EOF_CHAR);
        //404
        transform(404, '\'', Type::SINGLE_QOUTE);
        transform(404, '\\', Type::BACWARD_SLASH);
        transform(404, '0','7', Type::OCT_DIGIT);
        transform(404, '\n', Type::NEW_LINE);
        transform(404, '\r', Type::NEW_LINE);
        transform(404, EOF, Type::EOF_CHAR);
        //405
        transform(405, '\'', Type::SINGLE_QOUTE);
        transform(405, '\\', Type::BACWARD_SLASH);
        transform(405, '0', '7', Type::OCT_DIGIT);
        transform(405, '\n', Type::NEW_LINE);
        transform(405, '\r', Type::NEW_LINE);
        transform(405, EOF, Type::EOF_CHAR);
        //406
        transform(406, '\'', Type::SINGLE_QOUTE);
        transform(406, '\\', Type::BACWARD_SLASH);
        transform(406, '\n', Type::NEW_LINE);
        transform(406, '\r', Type::NEW_LINE);
        transform(406, EOF, Type::EOF_CHAR);
        //407
        transform(407, '0', '9', Type::HEX_DIGIT);
        transform(407, 'a', 'f', Type::HEX_DIGIT);
        transform(407, 'A', 'F', Type::HEX_DIGIT);
        transform(407, '\n', Type::NEW_LINE);
        transform(407, '\r', Type::NEW_LINE);
        transform(407, EOF, Type::EOF_CHAR);

        //408
        transform(408, '\'', Type::SINGLE_QOUTE);
        transform(408, '0', '9', Type::HEX_DIGIT);
        transform(408, 'a', 'f', Type::HEX_DIGIT);
        transform(408, 'A', 'F', Type::HEX_DIGIT);
        transform(408, '\n', Type::NEW_LINE);
        transform(408, '\r', Type::NEW_LINE);
        transform(408, EOF, Type::EOF_CHAR);
        //409
        transform(409, EOF, Type::EOF_CHAR);
        //410
        transform(410, EOF, Type::EOF_CHAR);
        //411
        //501
        transform(501, '*', Type::STAR);
        transform(501, '/', Type::FORWARD_SLASH);
        transform(501, '=', Type::OPERATOR);
        //502
        transform(502, -128, 126, Type::CHAR);
        transform(502, '\n', Type::NEW_LINE);
        //503
        transform(503, -128, 126, Type::CHAR);
        transform(503, '*', Type::STAR);
        //504
        transform(504, -128, 126, Type::CHAR);
        transform(504, '*', Type::STAR);
        transform(504, '/', Type::FORWARD_SLASH);
        //601
        transform(601, "+-~!*/%<>=&|^.:", Type::OPERATOR);
    }
}
