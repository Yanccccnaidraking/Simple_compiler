#include "Parser/lr_1.h"
#include<queue>
namespace Parser {


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
                        actionTable[j][item.lookahead] = "r" + std::to_string(proNo);
                    }
                    else if (!proNo)
                    {
                        actionTable[j][item.lookahead] = "acc";
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
                        actionTable[j][token] = "s"+std::to_string(i);
                        tempSets.insert(newItemSet);
                    }
                    else
                    {
                        int c = findItemSetKey(newItemSet);
                        if (c >= 0)
                            actionTable[j][token] = "s" + std::to_string(c);
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
    /*Parser::loadGotoTable("GOTOTable.dat");
    Parser::printGotoTable();*/
    Parser::loadActionTable("ActionTable.dat");
    Parser::printActionTable();
    return 0;
}