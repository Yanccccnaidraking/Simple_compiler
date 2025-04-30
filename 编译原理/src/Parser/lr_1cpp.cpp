#include "Parser/lr_1.h"
#include<queue>
namespace Parser {

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
    std::set<std::string> terminals = { "[","]","{", "}", ";", "(", ")", "+", "-", "*", "/", "!","=","==", "!=", "<", "<=", ">", ">=", "||", "&&", "$","num", "real", "true", "false","if","while","else","do","break","id","basic","" };
    std::set<std::string> nonTerminals = { "program", "block", "decls", "decl", "type", "stmts", "stmt", "loc", "bool", "join", "equality", "rel", "expr", "term", "unary", "factor" };
    std::map<std::string, std::set<std::string>> firstSets;
    std::unordered_map<int, std::unordered_map<std::string, int>> actionTable; // Action Table
    std::unordered_map<int, std::unordered_map<std::string, int>> gotoTable; // Goto Table
    std::unordered_map<int, std::set<Item>> itemSets; // ���

    /// <summary>
    /// ��ѯLR(1)�������Action
    /// </summary>
    /// <param name="state"></param>
    /// <param name="token"></param>
    /// <returns></returns>
    Action searchFromAction(int state, std::string token)
    {
        auto rowIt = actionTable.find(state);
        if (rowIt != actionTable.end()) {
            auto colIt = rowIt->second.find(token);
            if (colIt != rowIt->second.end()) {
                return decodeAction(colIt->second);
            }
        }
        return Action{ActionType::Error,0};
    }

    /// <summary>
    /// ��ѯLR(1)�������GOTO
    /// </summary>
    /// <param name="state"></param>
    /// <param name="token"></param>
    /// <returns></returns>
    int searchFromGoto(int state, std::string token)
    {
        auto rowIt = gotoTable.find(state);
        if (rowIt != gotoTable.end()) {
            auto colIt = rowIt->second.find(token);
            if (colIt != rowIt->second.end()) {
                return colIt->second;
            }
        }
        return -1;
    }

    void printFirstSets() {
        for (const auto& [nonTerminal, firstSet] : firstSets) {
            std::cout << "First(" << nonTerminal << ") = { ";
            for (const auto& symbol : firstSet) {
                std::cout << symbol << "  ";
            }
            std::cout << "}" << std::endl;
        }
    }

    void printItemSets() {
        for (const auto& pair : itemSets) {
            std::cout << "Key: " << pair.first << "\n";
            std::cout << "Items: ";
            // ����ÿ�� set<Item> ����ӡ��Ԫ��
            for (const auto& item : pair.second) {
                std::cout << Parser::toStringItem(item) << " \n";
            }
            std::cout << "\n" << std::endl;
        }
        std::cout << itemSets.size() << std::endl;
    }

    /// <summary>
    /// ��ʼ��ÿһ�����ս����First��
    /// </summary>
    inline void initFirstSets()
    {
        for (const auto& nonTerminal : nonTerminals) {
            firstSets[nonTerminal] = {};  // ֱ�ӳ�ʼ��Ϊ�ռ���
        }
    }

    /// <summary>
    /// �������з��ս���ŵ�First��
    /// </summary>
    void computeFirstSets()
    {
        initFirstSets();
        bool added = false;
        do
        {
            added = false;
            for (const auto& pro : grammar)
            {
                std::string nonTerminal = pro.left;
                bool nullable = true; // ��ǰǰ׺�Ƿ�����Ƴ��մ�
                for (const auto& token : pro.right)
                {
                    if (isTerminal(token))
                    {
                        if (firstSets[nonTerminal].insert(token).second)
                        {
                            added = true;
                        }
                        nullable = false;
                        break;//��ȡ���ս����ֱ���˳�
                    }
                    for (auto& f : firstSets[token]) {
                        if (f != "" && firstSets[nonTerminal].insert(f).second) {
                            added = true; // �������Ԫ�ؼ��룬��Ǳ仯
                        }
                    }
                    if (!firstSets[token].count(""))//û�ж�ȡ���շ��ţ����ټ�������ȡ
                    {
                        nullable = false;
                        break;
                    }
                }
                if (nullable) {
                    if (firstSets[nonTerminal].insert("").second) {
                        added = true;
                    }
                }
            }
        } while (added);
    }

    /// <summary>
    /// ����ĳһ������ʽ���壬�������Ӧ��First����
    /// </summary>
    /// <param name="lists"></param>
    /// <returns></returns>
    std::set<std::string> getFirstSetByVector(std::vector<std::string> lists)
    {
        std::set<std::string> results;
        bool nullable = true;
        for (const auto& item : lists)
        {
            if (item == "")
            {
                continue;
            }
            if (isTerminal(item))//��Ϊ�ս��������������
            {
                results.insert(item);
                nullable = false;
                break;
            }
            for (const auto& b : firstSets[item])
            {
                if (b != "")
                {
                    results.insert(b);
                }
            }
            if (!firstSets[item].count(""))
            {
                nullable = false;
                break;
            }
        }
        if (nullable)
        {
            results.insert("");
        }
        return results;
    }

    /// <summary>
    /// ����հ���closure�����ڹ�����Ŀ��
    /// </summary>
    /// <param name="items"></param>
    /// <returns></returns>
    std::set<Item> closure(std::set<Item>& items) {
        std::set<Item> tempItems;
        std::queue<Item> closureItems;//ת�ɶ��д洢
        for (const auto& item : items) {
            closureItems.push(item);
            tempItems.insert(item);
        }
        while (!closureItems.empty())
        {
            const auto& item = closureItems.front();
            items.insert(item);
            int size = item.production.right.size();
            if (item.dotPosition < size)//���滹�з���
            {
                std::string token = item.production.right[item.dotPosition];
                if (isNonTerminal(token))//ֻ�з��ս������Ҫ����
                {
                    for (const auto& pro : grammar)//����ÿ������ʽ
                    {
                        if (pro.left != token)
                            continue;
                        std::vector<std::string> tokens;
                        for (int i = item.dotPosition + 1; i < size; i++)
                        {
                            tokens.push_back(item.production.right[i]);
                        }
                        tokens.push_back(item.lookahead);
                        std::set<std::string> terminals = getFirstSetByVector(tokens);//�ҳ�First�������е��ս����
                        terminals.erase(""); // ɾ���մ���""��
                        for (const auto& terminal : terminals)
                        {
                            Item ii = Item({ Production{pro.left, pro.right}, 0, terminal });
                            if (items.find(ii) == items.end()&&tempItems.find(ii)==tempItems.end())//֮ǰ�����ڵĲ���ӽ���
                            {
                                closureItems.push(ii);
                            }
                        }
                    }
                }
            }
            closureItems.pop();
        }
        return items;
    }

    /// <summary>
    /// ������Ŀ����ת��
    /// </summary>
    /// <param name="items"></param>
    /// <param name="symbol"></param>
    /// <returns></returns>
    std::set<Item> goTo(const std::set<Item>& items, const std::string& symbol) {
        std::set<Item> result;
        for (const auto& item : items) {
            if (item.dotPosition < item.production.right.size() && item.production.right[item.dotPosition] == symbol) {
                Item newItem = item;
                newItem.dotPosition++;
                result.insert(newItem);
            }
        }
        return closure(result);
    }

    /// <summary>
    /// ����LR(1)���
    /// </summary>
    void genItems() {
        itemSets[0] = { Item({ Production{"program", {"block"}}, 0, "$" }) };//��ʼ��program->.block , $
        closure(itemSets[0]);//��հ�

        std::vector<std::set<Item>> arr;
        //std::queue<std::set<Item>> lists;
        std::set<std::set<Item>> tempSets;
        
        arr.push_back(itemSets[0]);

        //lists.push(itemSets[0]);
        tempSets.insert(itemSets[0]);
        int i = 0;
        //std::set<std::string> tokens;
        //tokens.insert(terminals.begin(), terminals.end());
        //tokens.insert(nonTerminals.begin(), nonTerminals.end());//�ϲ��ս���ͷ��ս��
        //while (!lists.empty())
        //{
        //    std::set<Item> itemSet = lists.front();
        //    //�ֳ��ս�ͷ��ս��������ֵ�ԭ������Ҫ�ֱ���Action���GOTO�����ʺϺϲ�����
        //    for (const auto& token : terminals)//�ս���Ų���
        //    {
        //        std::set<Item> newItemSet = goTo(itemSet, token);
        //        if (!newItemSet.empty())
        //        {
        //            if (tempSets.find(newItemSet) == tempSets.end())
        //            {
        //                lists.push(newItemSet);
        //                itemSets[i++] = newItemSet;
        //                tempSets.insert(newItemSet);
        //            }
        //        }
        //    }
        //    for (const auto& token : nonTerminals)//���ս���Ų���
        //    {
        //        std::set<Item> newItemSet = goTo(itemSet, token);
        //        if (!newItemSet.empty())
        //        {
        //            if (tempSets.find(newItemSet) == tempSets.end())
        //            {
        //                lists.push(newItemSet);
        //                itemSets[i++] = newItemSet;
        //                tempSets.insert(newItemSet);
        //            }
        //        }
        //    }
        //    lists.pop();
        //}
        for (int j = 0; j < arr.size(); j++)
        {
            std::set<Item> itemSet = arr[j];
            //�ֳ��ս�ͷ��ս��������ֵ�ԭ������Ҫ�ֱ���Action���GOTO�����ʺϺϲ�����

            //��дAction��
            // ���ڸ��ķ�����LR(1)�ķ�������Ҫ����һ����Լ����ʹ�䲻�������ͻ��
            // �ķ���������else�����⣬Ϊ�˽����һ���⣬��Ҫ������Ϊelse��ʱ��ѡ�����룬�����ǹ�Լ��
            // ���ڱ��ķ������������ĳ�ͻ�����Բ�ȡ�ȹ�Լ��������������ԣ������븲�ǹ�Լ���Խ����ͻ

            //��Լ����
            for (const auto& item : itemSet)
            {
                if (item.dotPosition == item.production.right.size())//�������Ҳ࣬˵��Ӧ�ù�Լ
                {
                    int proNo = getGrammarNo(item.production);
                    if (proNo > 0)
                    {
                        actionTable[j][item.lookahead] = encodeAction(ActionType::Reduce, proNo);
                    }
                    else if (!proNo)
                    {
                        actionTable[j][item.lookahead] = encodeAction(ActionType::Accept,0);
                    }
                }
            }

            for (const auto& token : terminals)//�ս���Ų���
            {
                if (token == "")
                    continue;
                std::set<Item> newItemSet = goTo(itemSet, token);
                if (!newItemSet.empty())
                {
                    if (tempSets.find(newItemSet) == tempSets.end())
                    {
                        arr.push_back(newItemSet);
                        itemSets[++i] = newItemSet;
                        actionTable[j][token] = encodeAction(ActionType::Shift,i);
                        tempSets.insert(newItemSet);
                    }
                    else
                    {
                        int c = findItemSetKey(newItemSet);
                        if (c >= 0)
                            actionTable[j][token] = encodeAction(ActionType::Shift,c);
                    }
                }
            }
            for (const auto& token : nonTerminals)//���ս���Ų���
            {
                std::set<Item> newItemSet = goTo(itemSet, token);
                if (!newItemSet.empty())
                {
                    if (tempSets.find(newItemSet) == tempSets.end())
                    {
                        arr.push_back(newItemSet);
                        itemSets[++i] = newItemSet;
                        gotoTable[j][token] = i;
                        tempSets.insert(newItemSet);
                    }
                    else
                    {
                        int c = findItemSetKey(newItemSet);
                        if (c>=0)
                            gotoTable[j][token] = c;
                    }
                }
            }
        }
    }

    /// <summary>
    /// ���������ķ���
    /// </summary>
    /// <param name="type"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    int encodeAction(ActionType type, int value) {
        return (static_cast<int>(type) << 28) | (value & 0x0FFFFFFF);
    }

    /// <summary>
    /// ����������action
    /// </summary>
    /// <param name="encoded"></param>
    /// <returns></returns>
    Action decodeAction(int encoded) {
        ActionType type = static_cast<ActionType>((encoded >> 28) & 0xF);
        int value = encoded & 0x0FFFFFFF;
        return { type, value };
    }

    void saveActionTable(const std::string& filename) {
        std::ofstream out(filename, std::ios::binary);
        if (!out) return;

        size_t outerSize = actionTable.size();
        out.write(reinterpret_cast<const char*>(&outerSize), sizeof(outerSize));

        for (const auto& [state, transitions] : actionTable) {
            out.write(reinterpret_cast<const char*>(&state), sizeof(state));

            size_t innerSize = transitions.size();
            out.write(reinterpret_cast<const char*>(&innerSize), sizeof(innerSize));

            for (const auto& [symbol, action] : transitions) {
                size_t keyLen = symbol.size();
                out.write(reinterpret_cast<const char*>(&keyLen), sizeof(keyLen));
                out.write(symbol.data(), keyLen);

                out.write(reinterpret_cast<const char*>(&action), sizeof(action));
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

            std::unordered_map<std::string, int> transitions;

            for (size_t j = 0; j < innerSize; ++j) {
                size_t keyLen;
                in.read(reinterpret_cast<char*>(&keyLen), sizeof(keyLen));

                std::string symbol(keyLen, '\0');
                in.read(&symbol[0], keyLen);

                int action;
                in.read(reinterpret_cast<char*>(&action), sizeof(action));

                transitions[symbol] = action;
            }

            actionTable[state] = std::move(transitions);
        }

        in.close();
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
}

int main()
{
    /*Parser::computeFirstSets();
    Parser::genItems();
    Parser::printItemSets();
    Parser::serializeItemSets("ItemSets.dat");
    Parser::saveGotoTable("GOTOTable.dat");
    Parser::saveActionTable("ActionTable.dat");*/
    /*Parser::deserializeItemSets("ItemSets.dat");
    Parser::printItemSets();*/
    Parser::loadGotoTable("GOTOTable.dat");
    Parser::printGotoTable();
    Parser::loadActionTable("ActionTable.dat");
    Parser::printActionTable();
    return 0;
}