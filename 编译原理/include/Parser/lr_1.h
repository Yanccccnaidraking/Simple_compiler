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
    std::set<std::string> terminals= { "[","]","{", "}", ";", "(", ")", "+", "-", "*", "/", "!","=","==", "!=", "<", "<=", ">", ">=", "||", "&&", "$","num", "real", "true", "false","if","while","else","do","break","id","basic",""};
    std::set<std::string> nonTerminals = { "program", "block", "decls", "decl", "type", "stmts", "stmt", "loc", "bool", "join", "equality", "rel", "expr", "term", "unary", "factor" };

    // �ж�һ�������Ƿ����ս��
    inline bool isTerminal(const std::string& symbol) {
        return terminals.find(symbol) != terminals.end();
    }

    // �ж�һ�������Ƿ��Ƿ��ս��
    inline bool isNonTerminal(const std::string& symbol) {
        return nonTerminals.find(symbol) != nonTerminals.end();
    }

    // �洢ÿ�����ս���� First ����
    std::map<std::string, std::set<std::string>> firstSets;

    // �洢���еĲ���ʽ
    std::vector<Production> grammar = {
        {"program", {"block"}},

        {"block", {"{", "decls", "stmts", "}"}},

        {"decls", {"decls", "decl"}},  // �ݹ������
        {"decls", {""}},  // �� ����յĲ���ʽ

        {"decl", {"type", "id", ";"}},

        {"type", {"type", "[","num","]"}},  // ��������
        {"type", {"basic"}},  // ��������

        {"stmts", {"stmts", "stmt"}},  // �ݹ�����
        {"stmts", {""}},  // �� ����յĲ���ʽ

        {"stmt", {"loc", "=", "bool", ";"}},

        {"stmt", {"if", "(", "bool", ")", "stmt"}},  // if ���
        {"stmt", {"if", "(", "bool", ")", "stmt", "else", "stmt"}},  // if-else ���
        {"stmt", {"while", "(", "bool", ")", "stmt"}},  // while ���
        {"stmt", {"do", "stmt", "while", "(", "bool", ")", ";"}},  // do-while ���
        {"stmt", {"break", ";"}},  // break ���
        {"stmt", {"block"}},  // �����

        {"loc", {"loc", "[","num","]"}},  // �������
        {"loc", {"id"}},  // ��ʶ��

        {"bool", {"bool", "||", "join"}},  // �߼���
        {"bool", {"join"}},  // ��������

        {"join", {"join", "&&", "equality"}},  // �߼���
        {"join", {"equality"}},  // �������

        {"equality", {"equality", "==", "rel"}},  // ����
        {"equality", {"equality", "!=", "rel"}},  // ������
        {"equality", {"rel"}},  // ��ϵ���ʽ

        {"rel", {"expr", "<", "expr"}},  // С��
        {"rel", {"expr", "<=", "expr"}},  // С�ڵ���
        {"rel", {"expr", ">=", "expr"}},  // ���ڵ���
        {"rel", {"expr", ">", "expr"}},  // ����
        {"rel", {"expr"}},  // ��һ���ʽ

        {"expr", {"expr", "+", "term"}},  // �ӷ�
        {"expr", {"expr", "-", "term"}},  // ����
        {"expr", {"term"}},  // ��һ��

        {"term", {"term", "*", "unary"}},  // �˷�
        {"term", {"term", "/", "unary"}},  // ����
        {"term", {"unary"}},  // ��һ����

        {"unary", {"!", "unary"}},  // �߼���
        {"unary", {"-", "unary"}},  // ����
        {"unary", {"factor"}},  // ��������

        {"factor", {"(", "bool", ")"}},  // �����ڵĲ������ʽ
        {"factor", {"loc"}},  // λ�ã�������
        {"factor", {"num"}},  // ���ֳ���
        {"factor", {"real"}},  // ����������
        {"factor", {"true"}},  // �������� true
        {"factor", {"false"}}  // �������� false
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
    std::unordered_map<int, std::set<Item>> itemSets; // ���

    int findItemSetKey(const std::set<Item>& targetSet) {
        for (const auto& [key, valueSet] : itemSets) {
            if (valueSet == targetSet) {
                return key; // �ҵ��˶�Ӧ��
            }
        }
        return -1; // δ�ҵ�
    }

    // ���л� itemSets ���ļ�
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

    // �����л� itemSets ���ļ�
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