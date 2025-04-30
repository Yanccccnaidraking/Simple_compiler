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

    enum class ActionType {
        Error = 0,
        Shift = 1,//移入操作
        Reduce = 2,//规约操作
        Accept = 3//接收
    };

    struct Action {
        ActionType type;
        int value; // 移入目标状态或规约产生式编号
    };


    int encodeAction(ActionType type, int value);


    Action decodeAction(int encoded);

    inline std::string getActionStr(ActionType type)
    {
        switch (type)
        {
        case ActionType::Error:
            return "error";
        case ActionType::Shift:
            return "Shift";
        case ActionType::Reduce:
            return "Reduce";
        case ActionType::Accept:
            return "Accept";
        default:
            return "";
        }
    }

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
    extern std::set<std::string> terminals;
    extern std::set<std::string> nonTerminals;

    // 判断一个符号是否是终结符
    inline bool isTerminal(const std::string& symbol) {
        return terminals.find(symbol) != terminals.end();
    }

    // 判断一个符号是否是非终结符
    inline bool isNonTerminal(const std::string& symbol) {
        return nonTerminals.find(symbol) != nonTerminals.end();
    }

    // 存储每个非终结符的 First 集合
    extern std::map<std::string, std::set<std::string>> firstSets;

    // 存储所有的产生式
    extern std::vector<Production> grammar;

    inline int getGrammarNo(Production pro)
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

    inline std::string toStringItem(Item item)
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

    extern std::unordered_map<int, std::unordered_map<std::string, int>> actionTable; // Action Table
    extern std::unordered_map<int, std::unordered_map<std::string, int>> gotoTable; // Goto Table
    extern std::unordered_map<int, std::set<Item>> itemSets; // 项集族

    inline int findItemSetKey(const std::set<Item>& targetSet) {
        for (const auto& [key, valueSet] : itemSets) {
            if (valueSet == targetSet) {
                return key; // 找到了对应键
            }
        }
        return -1; // 未找到
    }

    // 序列化 itemSets 到文件
    void serializeItemSets(const std::string& filename);

    // 反序列化 itemSets 从文件
    void deserializeItemSets(const std::string& filename);

    void saveActionTable(const std::string& filename);

    void loadActionTable(const std::string& filename);


    void saveGotoTable(const std::string& filename);

    void loadGotoTable(const std::string& filename);

    inline void printActionTable() {
        std::cout << "=== ACTION TABLE ===" << std::endl;
        for (const auto& [state, transitions] : actionTable) {
            std::cout << "State " << state << ":" << std::endl;
            for (const auto& [symbol, action] : transitions) {
                Action act = decodeAction(action);
                std::cout << "  On '" << symbol << "' -> " << getActionStr(act.type)<<act.value << std::endl;
            }
        }
        std::cout << std::endl;
    }

    inline void printGotoTable() {
        std::cout << "=== GOTO TABLE ===" << std::endl;
        for (const auto& [state, transitions] : gotoTable) {
            std::cout << "State " << state << ":" << std::endl;
            for (const auto& [symbol, nextState] : transitions) {
                std::cout << "  On '" << symbol << "' -> " << nextState << std::endl;
            }
        }
        std::cout << std::endl;
    }

    Action searchFromAction(int state, std::string token);

    int searchFromGoto(int state, std::string token);

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