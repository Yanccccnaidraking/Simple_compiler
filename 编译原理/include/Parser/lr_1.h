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
        Shift = 1,//�������
        Reduce = 2,//��Լ����
        Accept = 3//����
    };

    struct Action {
        ActionType type;
        int value; // ����Ŀ��״̬���Լ����ʽ���
    };

    /// <summary>
    /// �﷨��������ı����
    /// </summary>
    struct ParserItem {
        SmartArray<int> stack;
        std::string symbol;
        std::string input;
        std::string action;
    };

    extern std::vector<ParserItem> parserTable;

    /// <summary>
    /// �����﷨�����������
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

    // ������������ջ����ת��Ϊ�ַ������� bottom �� top��
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

    // ��ӡ����
    inline void writeParserTableToFile(const std::string& filename) {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "�޷����ļ���" << filename << std::endl;
            return;
        }

        const int width_stack = 80;
        const int width_symbol = 80;
        const int width_input = 10;
        const int width_action = 20;

        // ��ͷ
        outFile << std::left
            << std::setw(width_stack) << "ջ"
            << std::setw(width_symbol) << "����"
            << std::setw(width_input) << "����"
            << std::setw(width_action) << "����"
            << "\n";

        outFile << std::string(width_stack + width_symbol + width_input + width_action, '-') << "\n";

        // �������
        for (const auto& item : parserTable) {
            outFile << std::left
                << std::setw(width_stack) << stackToString(item.stack)
                << std::setw(width_symbol) << item.symbol
                << std::setw(width_input) << item.input
                << std::setw(width_action) << item.action
                << "\n";
        }

        outFile.close();
        std::cout << "����ѳɹ�д�뵽�ļ���" << filename << std::endl;
    }

    // д�� CSV �ļ�
    inline void writeParserCSV( const std::string& filename) {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "�޷����ļ���" << filename << std::endl;
            return;
        }

        // ��ͷ
        outFile << "Stack,Symbol,Input,Action\n";

        for (const auto& item : parserTable) {
            // ����ֶ����ж��Ż�ո񣬿��Լ����ţ���ѡ��ǿ��
            outFile << "\"" << stackToString(item.stack) << "\","
                << "\"" << item.symbol << "\","
                << "\"" << item.input << "\","
                << "\"" << item.action << "\""
                << "\n";
        }

        outFile.close();
        std::cout << "CSV �ļ�д��ɹ���" << filename << std::endl;
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
        std::string left;//����ʽ��ͷ
        std::vector<std::string> right;//����ʽ����
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

        // ���л�Production
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

        // �����л�Production
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

    // ȫ�ֵ��ս���ͷ��ս������
    extern std::set<std::string> terminals;
    extern std::set<std::string> nonTerminals;

    // �ж�һ�������Ƿ����ս��
    inline bool isTerminal(const std::string& symbol) {
        return terminals.find(symbol) != terminals.end();
    }

    // �ж�һ�������Ƿ��Ƿ��ս��
    inline bool isNonTerminal(const std::string& symbol) {
        return nonTerminals.find(symbol) != nonTerminals.end();
    }

    void computeFirstSets();

    void printFirstSets();

    void printItemSets();

    // �洢ÿ�����ս���� First ����
    extern std::map<std::string, std::set<std::string>> firstSets;

    // �洢���еĲ���ʽ
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
        int dotPosition; // Բ��λ�ã���ʾĿǰ����������ʽ����һ����
        std::string lookahead; // ��һ���ţ�����LR(1)��

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

        // ���л�Item
        void serialize(std::ofstream& out) const {
            production.serialize(out);
            out.write(reinterpret_cast<const char*>(&dotPosition), sizeof(dotPosition));
            size_t lookaheadSize = lookahead.size();
            out.write(reinterpret_cast<const char*>(&lookaheadSize), sizeof(lookaheadSize));
            out.write(lookahead.c_str(), lookaheadSize);
        }

        // �����л�Item
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
    extern std::unordered_map<int, std::set<Item>> itemSets; // ���

    inline int findItemSetKey(const std::set<Item>& targetSet) {
        for (const auto& [key, valueSet] : itemSets) {
            if (valueSet == targetSet) {
                return key; // �ҵ��˶�Ӧ��
            }
        }
        return -1; // δ�ҵ�
    }

    // ���л� itemSets ���ļ�
    void serializeItemSets(const std::string& filename);

    // �����л� itemSets ���ļ�
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
// Ϊ���ܹ���unordered_map��ʹ��Item��Ϊkey����Ҫ�����ϣ����
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