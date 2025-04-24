#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include "Lexer/lexer.h"
#include "Lexer/Num.h"
#include "Lexer/Real.h"
#include "Lexer/Tag.h"


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
    {Lexer::Tag::SWITCH, "SWITCH"},
    {Lexer::Tag::TEMP, "TEMP"},
    {Lexer::Tag::TRUE, "TRUE"},
    {Lexer::Tag::WHILE, "WHILE"}
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
    // 打开代码文本文件
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return 1;
    }

    // 重定向标准输入到文件
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::cin.rdbuf(file.rdbuf());

    // 创建Lexer对象
    Lexer::Lexer lexer=Lexer::Lexer("input.txt");

    // 扫描并输出词法单元
    std::shared_ptr<Lexer::Token> token;
    while ((token = lexer.scan()) != nullptr) {
        if (auto num = std::dynamic_pointer_cast<Lexer::Num>(token)) {
            std::cout << "<" << num->toString() << ", " << getEnumString((Lexer::Tag)num->tag) << ">" << std::endl;
        }
        else if (auto real = std::dynamic_pointer_cast<Lexer::Real>(token)) {
            std::cout << "<" << real->toString() << ", " << getEnumString((Lexer::Tag)real->tag) << ">" << std::endl;
        }
        else if (auto word = std::dynamic_pointer_cast<Lexer::Word>(token)) {
            std::cout << "<" << word->toString() << ", " << getEnumString((Lexer::Tag)word->tag) << ">" << std::endl;
        }
        else {
            std::cout << "<" << token->toString() << ">" << std::endl;
        }
    }

    // 恢复标准输入
    std::cin.rdbuf(cinbuf);
    file.close();

    return 0;
}
