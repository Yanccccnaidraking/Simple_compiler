#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <set>
#include <string>
#include <stack>
#include <algorithm>
#include<map>


namespace Parser {
    struct Production {
        std::string left;//产生式的头
        std::vector<std::string> right;//产生式的体
        bool operator==(const Production& other) const {
            return left == other.left && right == other.right;
        }

        bool operator!=(const Production& other) const {
            return left != other.left || right != other.right;
        }

        bool operator<(const Production& other) const {
            if (left != other.left) return left < other.left;
            return right < other.right;
        }

        // 序列化Production
        void serialize(std::ofstream& out) const {
            size_t leftSize = left.size();
            out.write(reinterpret_cast<const char*>(&leftSize), sizeof(leftSize));
            out.write(left.c_str(), leftSize);

            size_t rightSize = right.size();
            out.write(reinterpret_cast<const char*>(&rightSize), sizeof(rightSize));
            for (const auto& symbol : right) {
                size_t symbolSize = symbol.size();
                out.write(reinterpret_cast<const char*>(&symbolSize), sizeof(symbolSize));
                out.write(symbol.c_str(), symbolSize);
            }
        }

        // 反序列化Production
        void deserialize(std::ifstream& in) {
            size_t leftSize;
            in.read(reinterpret_cast<char*>(&leftSize), sizeof(leftSize));
            left.resize(leftSize);
            in.read(&left[0], leftSize);

            size_t rightSize;
            in.read(reinterpret_cast<char*>(&rightSize), sizeof(rightSize));
            right.resize(rightSize);
            for (auto& symbol : right) {
                size_t symbolSize;
                in.read(reinterpret_cast<char*>(&symbolSize), sizeof(symbolSize));
                symbol.resize(symbolSize);
                in.read(&symbol[0], symbolSize);
            }
        }
    };

    // 全局的终结符和非终结符集合
    std::set<std::string> terminals= { "[","]","{", "}", ";", "(", ")", "+", "-", "*", "/", "!","=","==", "!=", "<", "<=", ">", ">=", "||", "&&", "$","num", "real", "true", "false","if","while","else","do","break","id","basic",""};
    std::set<std::string> nonTerminals = { "program", "block", "decls", "decl", "type", "stmts", "stmt", "loc", "bool", "join", "equality", "rel", "expr", "term", "unary", "factor" };

    // 判断一个符号是否是终结符
    inline bool isTerminal(const std::string& symbol) {
        return terminals.find(symbol) != terminals.end();
    }

    // 判断一个符号是否是非终结符
    inline bool isNonTerminal(const std::string& symbol) {
        return nonTerminals.find(symbol) != nonTerminals.end();
    }

    // 存储每个非终结符的 First 集合
    std::map<std::string, std::set<std::string>> firstSets;

    // 存储所有的产生式
    std::vector<Production> grammar = {
        {"program", {"block"}},

        {"block", {"{", "decls", "stmts", "}"}},

        {"decls", {"decls", "decl"}},  // 递归的声明
        {"decls", {""}},  // ε 代表空的产生式

        {"decl", {"type", "id", ";"}},

        {"type", {"type", "[","num","]"}},  // 数组类型
        {"type", {"basic"}},  // 基本类型

        {"stmts", {"stmts", "stmt"}},  // 递归的语句
        {"stmts", {""}},  // ε 代表空的产生式

        {"stmt", {"loc", "=", "bool", ";"}},

        {"stmt", {"if", "(", "bool", ")", "stmt"}},  // if 语句
        {"stmt", {"if", "(", "bool", ")", "stmt", "else", "stmt"}},  // if-else 语句
        {"stmt", {"while", "(", "bool", ")", "stmt"}},  // while 语句
        {"stmt", {"do", "stmt", "while", "(", "bool", ")", ";"}},  // do-while 语句
        {"stmt", {"break", ";"}},  // break 语句
        {"stmt", {"block"}},  // 块语句

        {"loc", {"loc", "[","num","]"}},  // 数组访问
        {"loc", {"id"}},  // 标识符

        {"bool", {"bool", "||", "join"}},  // 逻辑或
        {"bool", {"join"}},  // 连接条件

        {"join", {"join", "&&", "equality"}},  // 逻辑与
        {"join", {"equality"}},  // 相等条件

        {"equality", {"equality", "==", "rel"}},  // 等于
        {"equality", {"equality", "!=", "rel"}},  // 不等于
        {"equality", {"rel"}},  // 关系表达式

        {"rel", {"expr", "<", "expr"}},  // 小于
        {"rel", {"expr", "<=", "expr"}},  // 小于等于
        {"rel", {"expr", ">=", "expr"}},  // 大于等于
        {"rel", {"expr", ">", "expr"}},  // 大于
        {"rel", {"expr"}},  // 单一表达式

        {"expr", {"expr", "+", "term"}},  // 加法
        {"expr", {"expr", "-", "term"}},  // 减法
        {"expr", {"term"}},  // 单一项

        {"term", {"term", "*", "unary"}},  // 乘法
        {"term", {"term", "/", "unary"}},  // 除法
        {"term", {"unary"}},  // 单一运算

        {"unary", {"!", "unary"}},  // 逻辑非
        {"unary", {"-", "unary"}},  // 负号
        {"unary", {"factor"}},  // 基本因子

        {"factor", {"(", "bool", ")"}},  // 括号内的布尔表达式
        {"factor", {"loc"}},  // 位置（变量）
        {"factor", {"num"}},  // 数字常量
        {"factor", {"real"}},  // 浮点数常量
        {"factor", {"true"}},  // 布尔常量 true
        {"factor", {"false"}}  // 布尔常量 false
    };

    int getGrammarNo(Production pro)
    {
        int size = grammar.size();
        for (int i = 0; i < size; i++)
        {
            if (pro == grammar[i])
            {
                return i;
            }
        }
        return -1;
    }

    struct Item {
        Production production;
        int dotPosition; // 圆点位置，表示目前分析到产生式的哪一部分
        std::string lookahead; // 下一符号（用于LR(1)）

        bool operator==(const Item& other) const {
            return production.left == other.production.left &&
                production.right == other.production.right &&
                dotPosition == other.dotPosition &&
                lookahead == other.lookahead;
        }

        bool operator<(const Item& other) const {
            if (production != other.production) return production < other.production;
            if (dotPosition != other.dotPosition) return dotPosition < other.dotPosition;
            return lookahead < other.lookahead;
        }

        // 序列化Item
        void serialize(std::ofstream& out) const {
            production.serialize(out);
            out.write(reinterpret_cast<const char*>(&dotPosition), sizeof(dotPosition));
            size_t lookaheadSize = lookahead.size();
            out.write(reinterpret_cast<const char*>(&lookaheadSize), sizeof(lookaheadSize));
            out.write(lookahead.c_str(), lookaheadSize);
        }

        // 反序列化Item
        void deserialize(std::ifstream& in) {
            production.deserialize(in);
            in.read(reinterpret_cast<char*>(&dotPosition), sizeof(dotPosition));
            size_t lookaheadSize;
            in.read(reinterpret_cast<char*>(&lookaheadSize), sizeof(lookaheadSize));
            lookahead.resize(lookaheadSize);
            in.read(&lookahead[0], lookaheadSize);
        }

    };

    std::string toStringItem(Item item)
    {
        std::string result = "";
        for (int i = 0; i < item.production.right.size(); i++)
        {
            if (i == item.dotPosition)
            {
                result += ".";
            }
            result += item.production.right[i]+" ";
        }
        if (item.dotPosition == item.production.right.size())
        {
            result += ".";
        }
        return item.production.left + " -> " + result + " , " + item.lookahead;
    }

    std::unordered_map<int, std::unordered_map<std::string, std::string>> actionTable; // Action Table
    std::unordered_map<int, std::unordered_map<std::string, int>> gotoTable; // Goto Table
    std::unordered_map<int, std::set<Item>> itemSets; // 项集族

    int findItemSetKey(const std::set<Item>& targetSet) {
        for (const auto& [key, valueSet] : itemSets) {
            if (valueSet == targetSet) {
                return key; // 找到了对应键
            }
        }
        return -1; // 未找到
    }

    // 序列化 itemSets 到文件
    void serializeItemSets(const std::string& filename) {
        std::ofstream out(filename, std::ios::binary);
        if (!out) {
            std::cerr << "Failed to open file for writing\n";
            return;
        }

        size_t mapSize = itemSets.size();
        out.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

        for (const auto& [state, items] : itemSets) {
            out.write(reinterpret_cast<const char*>(&state), sizeof(state));

            size_t setSize = items.size();
            out.write(reinterpret_cast<const char*>(&setSize), sizeof(setSize));

            for (const auto& item : items) {
                item.serialize(out);
            }
        }

        out.close();
    }

    // 反序列化 itemSets 从文件
    void deserializeItemSets(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary);
        if (!in) {
            std::cerr << "Failed to open file for reading\n";
            return;
        }

        size_t mapSize;
        in.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

        for (size_t i = 0; i < mapSize; ++i) {
            int state;
            in.read(reinterpret_cast<char*>(&state), sizeof(state));

            size_t setSize;
            in.read(reinterpret_cast<char*>(&setSize), sizeof(setSize));

            std::set<Item> items;
            for (size_t j = 0; j < setSize; ++j) {
                Item item;
                item.deserialize(in);
                items.insert(item);
            }

            itemSets[state] = std::move(items);
        }

        in.close();
    }

    void saveActionTable(const std::string& filename) {
        std::ofstream out(filename, std::ios::binary);
        if (!out) return;

        size_t outerSize = actionTable.size();
        out.write(reinterpret_cast<char*>(&outerSize), sizeof(outerSize));

        for (const auto& [state, transitions] : actionTable) {
            out.write(reinterpret_cast<const char*>(&state), sizeof(state));

            size_t innerSize = transitions.size();
            out.write(reinterpret_cast<const char*>(&innerSize), sizeof(innerSize));

            for (const auto& [symbol, action] : transitions) {
                size_t keyLen = symbol.size();
                size_t valLen = action.size();

                out.write(reinterpret_cast<const char*>(&keyLen), sizeof(keyLen));
                out.write(symbol.data(), keyLen);

                out.write(reinterpret_cast<const char*>(&valLen), sizeof(valLen));
                out.write(action.data(), valLen);
            }
        }

        out.close();
    }

    void loadActionTable(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary);
        if (!in) return;

        actionTable.clear();

        size_t outerSize;
        in.read(reinterpret_cast<char*>(&outerSize), sizeof(outerSize));

        for (size_t i = 0; i < outerSize; ++i) {
            int state;
            in.read(reinterpret_cast<char*>(&state), sizeof(state));

            size_t innerSize;
            in.read(reinterpret_cast<char*>(&innerSize), sizeof(innerSize));

            std::unordered_map<std::string, std::string> transitions;

            for (size_t j = 0; j < innerSize; ++j) {
                size_t keyLen, valLen;

                in.read(reinterpret_cast<char*>(&keyLen), sizeof(keyLen));
                std::string symbol(keyLen, '\0');
                in.read(&symbol[0], keyLen);

                in.read(reinterpret_cast<char*>(&valLen), sizeof(valLen));
                std::string action(valLen, '\0');
                in.read(&action[0], valLen);

                transitions[symbol] = action;
            }

            actionTable[state] = std::move(transitions);
        }

        in.close();
    }

    void saveGotoTable(const std::string& filename) {
        std::ofstream out(filename, std::ios::binary);
        if (!out) return;

        size_t outerSize = gotoTable.size();
        out.write(reinterpret_cast<char*>(&outerSize), sizeof(outerSize));

        for (const auto& [state, transitions] : gotoTable) {
            out.write(reinterpret_cast<const char*>(&state), sizeof(state));

            size_t innerSize = transitions.size();
            out.write(reinterpret_cast<const char*>(&innerSize), sizeof(innerSize));

            for (const auto& [symbol, nextState] : transitions) {
                size_t keyLen = symbol.size();
                out.write(reinterpret_cast<const char*>(&keyLen), sizeof(keyLen));
                out.write(symbol.data(), keyLen);
                out.write(reinterpret_cast<const char*>(&nextState), sizeof(nextState));
            }
        }

        out.close();
    }

    void loadGotoTable(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary);
        if (!in) return;

        gotoTable.clear();

        size_t outerSize;
        in.read(reinterpret_cast<char*>(&outerSize), sizeof(outerSize));

        for (size_t i = 0; i < outerSize; ++i) {
            int state;
            in.read(reinterpret_cast<char*>(&state), sizeof(state));

            size_t innerSize;
            in.read(reinterpret_cast<char*>(&innerSize), sizeof(innerSize));

            std::unordered_map<std::string, int> transitions;

            for (size_t j = 0; j < innerSize; ++j) {
                size_t keyLen;
                in.read(reinterpret_cast<char*>(&keyLen), sizeof(keyLen));

                std::string symbol(keyLen, '\0');
                in.read(&symbol[0], keyLen);

                int nextState;
                in.read(reinterpret_cast<char*>(&nextState), sizeof(nextState));

                transitions[symbol] = nextState;
            }

            gotoTable[state] = std::move(transitions);
        }

        in.close();
    }

    void printActionTable() {
        std::cout << "=== ACTION TABLE ===" << std::endl;
        for (const auto& [state, transitions] : actionTable) {
            std::cout << "State " << state << ":" << std::endl;
            for (const auto& [symbol, action] : transitions) {
                std::cout << "  On '" << symbol << "' -> " << action << std::endl;
            }
        }
        std::cout << std::endl;
    }

    void printGotoTable() {
        std::cout << "=== GOTO TABLE ===" << std::endl;
        for (const auto& [state, transitions] : gotoTable) {
            std::cout << "State " << state << ":" << std::endl;
            for (const auto& [symbol, nextState] : transitions) {
                std::cout << "  On '" << symbol << "' -> " << nextState << std::endl;
            }
        }
        std::cout << std::endl;
    }


}
// 为了能够在unordered_map中使用Item作为key，需要定义哈希函数
namespace std {
    template <>
    struct hash<Parser::Item> {
        size_t operator()(const Parser::Item& item) const {
            size_t hashValue = 0;
            auto hash_combine = [](size_t& seed, const auto& v) {
                seed ^= std::hash<std::decay_t<decltype(v)>>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
                };

            hash_combine(hashValue, item.production.left);
            for (const auto& symbol : item.production.right) {
                hash_combine(hashValue, symbol);
            }
            hash_combine(hashValue, item.dotPosition);
            hash_combine(hashValue, item.lookahead);
            return hashValue;
        }
    };
}