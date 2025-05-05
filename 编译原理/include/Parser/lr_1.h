#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <set>
#include <string>
#include <stack>
#include <algorithm>
#include <iomanip>
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

    /// <summary>
    /// 语法分析输出的表格项
    /// </summary>
    struct ParserItem {
        SmartArray<int> stack;
        std::string symbol;
        std::string input;
        std::string action;
    };

    extern std::vector<ParserItem> parserTable;

    /// <summary>
    /// 构造语法分析的输出表
    /// </summary>
    /// <param name="s"></param>
    /// <param name="symbol"></param>
    /// <param name="input"></param>
    /// <param name="pro"></param>
    inline void insertParserTable(SmartArray<int> s, std::deque<std::string> symbols, std::string input, std::string action)
    {
        std::string symbol;
        for (const auto& s : symbols)
        {
            symbol += s;
        }
        parserTable.push_back(ParserItem{ s,symbol,input,action});
    }

    // 辅助函数：将栈内容转换为字符串（从 bottom 到 top）
    inline std::string stackToString(SmartArray<int> s) {
        std::vector<int> temp;
        temp = s.getVector();
        std::ostringstream oss;
        for (int i = 0; i < temp.size(); ++i) {
            oss << temp[i];
            if (i != temp.size() - 1) oss << " ";
        }
        return oss.str();
    }

    // 打印函数
    inline void writeParserTableToFile(const std::string& filename) {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "无法打开文件：" << filename << std::endl;
            return;
        }

        const int width_stack = 80;
        const int width_symbol = 80;
        const int width_input = 10;
        const int width_action = 20;

        // 表头
        outFile << std::left
            << std::setw(width_stack) << "栈"
            << std::setw(width_symbol) << "符号"
            << std::setw(width_input) << "输入"
            << std::setw(width_action) << "动作"
            << "\n";

        outFile << std::string(width_stack + width_symbol + width_input + width_action, '-') << "\n";

        // 表格数据
        for (const auto& item : parserTable) {
            outFile << std::left
                << std::setw(width_stack) << stackToString(item.stack)
                << std::setw(width_symbol) << item.symbol
                << std::setw(width_input) << item.input
                << std::setw(width_action) << item.action
                << "\n";
        }

        outFile.close();
        std::cout << "表格已成功写入到文件：" << filename << std::endl;
    }

    // 写入 CSV 文件
    inline void writeParserCSV( const std::string& filename) {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "无法打开文件：" << filename << std::endl;
            return;
        }

        // 表头
        outFile << "Stack,Symbol,Input,Action\n";

        for (const auto& item : parserTable) {
            // 如果字段中有逗号或空格，可以加引号（可选增强）
            outFile << "\"" << stackToString(item.stack) << "\","
                << "\"" << item.symbol << "\","
                << "\"" << item.input << "\","
                << "\"" << item.action << "\""
                << "\n";
        }

        outFile.close();
        std::cout << "CSV 文件写入成功：" << filename << std::endl;
    }

    int encodeAction(ActionType type, int value);


    Action decodeAction(int encoded);

    inline std::string getActionStr(ActionType type)
    {
        switch (type)
        {
        case ActionType::Error:
            return "Error";
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

    void computeFirstSets();

    void printFirstSets();

    void printItemSets();

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

    inline std::string toStringProduction(Production production)
    {
        std::string result = "";
        for (int i = 0; i < production.right.size(); i++)
        {
            result += production.right[i] + " ";
        }
        return production.left + " -> " + result;
    }

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