#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include "Lexer/lexer.h"
#include "Lexer/Num.h"
#include "Lexer/Real.h"
#include "Lexer/String.h"
#include "Lexer/Char.h"
#include "Lexer/Tag.h"
#include "Parser/parser.h"
#include "Parser/lr_1.h"


// 定义映射
std::map<Lexer::Tag, std::string> tagToString = {
    {Lexer::Tag::AND, "AND"},
    {Lexer::Tag::BASIC, "BASIC"},
    {Lexer::Tag::BREAK, "BREAK"},
    {Lexer::Tag::CASE,"CASE"},
    {Lexer::Tag::DEFAULT,"DEFAULT"},
    {Lexer::Tag::DO, "DO"},
    {Lexer::Tag::ELSE, "ELSE"},
    {Lexer::Tag::EQ, "EQ"},
    {Lexer::Tag::FALSE, "FALSE"},
    {Lexer::Tag::FOR,"FOR"},
    {Lexer::Tag::GE, "GE"},
    {Lexer::Tag::ID, "ID"},
    {Lexer::Tag::IF, "IF"},
    {Lexer::Tag::INDEX, "INDEX"},
    {Lexer::Tag::LE, "LE"},
    {Lexer::Tag::MINUS, "MINUS"},
    {Lexer::Tag::NE, "NE"},
    {Lexer::Tag::NUM, "NUM"},
    {Lexer::Tag::OR, "OR"},
    {Lexer::Tag::REAL, "REAL"},
    {Lexer::Tag::RETURN, "RETURN"},
    {Lexer::Tag::SWITCH, "SWITCH"},
    {Lexer::Tag::TEMP, "TEMP"},
    {Lexer::Tag::TRUE, "TRUE"},
    {Lexer::Tag::VOID, "VOID"},
    {Lexer::Tag::WHILE, "WHILE"},
    {Lexer::Tag::PLUS_PLUS, "PLUS_PLUS"},
    {Lexer::Tag::MINUS_MINUS, "MINUS_MINUS"},
    {Lexer::Tag::PLUS_EQ, "PLUS_EQ"},
    {Lexer::Tag::MINUS_EQ, "MINUS_EQ"},
    {Lexer::Tag::MULT_EQ, "MULT_EQ"},
    {Lexer::Tag::DIV_EQ, "DIV_EQ"},
    {Lexer::Tag::MOD_EQ, "MOD_EQ"},
    {Lexer::Tag::AND_EQ, "AND_EQ"},
    {Lexer::Tag::OR_EQ, "OR_EQ"},
    {Lexer::Tag::XOR_EQ, "XOR_EQ"},
    {Lexer::Tag::LEFT_SHIFT, "LEFT_SHIFT"},
    {Lexer::Tag::RIGHT_SHIFT, "RIGHT_SHIFT"},
    {Lexer::Tag::ARROW, "ARROW"},
    {Lexer::Tag::STRING,"STRING"},
    {Lexer::Tag::CHAR,"CHAR"},
    {Lexer::Tag::CONTINUE,"CONTINUE"},
    {Lexer::Tag::SWITCH,"SWITCH"},
};

// 获取枚举对应的字符串
static std::string getEnumString(Lexer::Tag tag) {
    auto it = tagToString.find(tag);
    if (it != tagToString.end()) {
        return it->second;
    }
    return "UNKNOWN";
}

int main() {
    //词法分析部分输出
    // 创建Lexer对象
    Lexer::Lexer lexer = Lexer::Lexer("input.txt");
    // 扫描并输出词法单元
    std::shared_ptr<Lexer::Token> token;

    while ((token = lexer.scan())->tag!=(int)Lexer::Tag::END) {
        if (auto num = std::dynamic_pointer_cast<Lexer::Num>(token)) {
            std::cout << "<" << num->toString() << ", " << getEnumString((Lexer::Tag)num->tag) << ">" << std::endl;
        }
        else if (auto real = std::dynamic_pointer_cast<Lexer::Real>(token)) {
            std::cout << "<" << real->toString() << ", " << getEnumString((Lexer::Tag)real->tag) << ">" << std::endl;
        }
        else if (auto word = std::dynamic_pointer_cast<Lexer::Word>(token)) {
            std::cout << "<" << word->toString() << ", " << getEnumString((Lexer::Tag)word->tag) << ">" << std::endl;
        }
        else if (auto str = std::dynamic_pointer_cast<Lexer::String>(token)) {
            std::cout << "<" << str->toString() << ", " << getEnumString((Lexer::Tag)str->tag) << ">" << std::endl;
        }
        else if (auto str = std::dynamic_pointer_cast<Lexer::Char>(token)) {
            std::cout << "<" << str->toString() << ", " << getEnumString((Lexer::Tag)str->tag) << ">" << std::endl;
        }
        else {
            std::cout << "<" << token->toString() <<","<<token->toString() << ">" << std::endl;
        }
    }
    // 输出错误信息
    std::cout << "\n错误信息：\n" << "\033[31m" << lexer.error_info << "\033[0m" << std::endl;

    ////语法分析阶段输出
    //Lexer::Lexer lexer = Lexer::Lexer("input.txt");
    //Parser::Parser parser = Parser::Parser(lexer);
    //parser.program();
    //Parser::writeParserCSV("parsertable.csv");
    //parser.showScopes();
    return 0;
}
