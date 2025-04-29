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
        bool added = false;
        itemSets[0] = { Item({ Production{"program", {"block"}}, 0, "$" }) };//��ʼ��program->.block , $
        closure(itemSets[0]);//��հ�

        std::queue<std::set<Item>> lists;
        std::set<std::set<Item>> tempSets;

        lists.push(itemSets[0]);
        tempSets.insert(itemSets[0]);
        int i = 0;
        std::set<std::string> tokens;
        tokens.insert(terminals.begin(), terminals.end());
        tokens.insert(nonTerminals.begin(), nonTerminals.end());//�ϲ��ս���ͷ��ս��
        while (!lists.empty())
        {
            std::set<Item> itemSet = lists.front();
            itemSets[i++] = itemSet;
            std::cout << "��ǰ��i��ֵ��" << i << std::endl;
            for (const auto& token : tokens)
            {
                std::set<Item> newItemSet = goTo(itemSet, token);
                if (!newItemSet.empty())
                {
                    if (tempSets.find(newItemSet) == tempSets.end())
                    {
                        lists.push(newItemSet);
                        tempSets.insert(newItemSet);
                    }
                }
            }
            lists.pop();
        }
    }
}

int main()
{
    Parser::computeFirstSets();
    Parser::genItems();
    Parser::printItemSets();
    return 0;
}