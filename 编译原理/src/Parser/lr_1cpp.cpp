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
            // 遍历每个 set<Item> 并打印其元素
            for (const auto& item : pair.second) {
                std::cout << Parser::toStringItem(item) << " \n";
            }
            std::cout << "\n" << std::endl;
        }
    }

    /// <summary>
    /// 初始化每一个非终结符的First集
    /// </summary>
    inline void initFirstSets()
    {
        for (const auto& nonTerminal : nonTerminals) {
            firstSets[nonTerminal] = {};  // 直接初始化为空集合
        }
    }

    /// <summary>
    /// 计算所有非终结符号的First集
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
                bool nullable = true; // 当前前缀是否可以推出空串
                for (const auto& token : pro.right)
                {
                    if (isTerminal(token))
                    {
                        if (firstSets[nonTerminal].insert(token).second)
                        {
                            added = true;
                        }
                        break;//读取到终结符，直接退出
                    }
                    for (auto& f : firstSets[token]) {
                        if (f != "" && firstSets[nonTerminal].insert(f).second) {
                            added = true; // 如果有新元素加入，标记变化
                        }
                    }
                    if (!firstSets[token].count(""))//没有读取到空符号，不再继续向后读取
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
            if (isTerminal(item))//若为终结符，则不再向后遍历
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
    /// 计算闭包（closure）用于构建项目集
    /// </summary>
    /// <param name="items"></param>
    /// <returns></returns>
    std::set<Item> closure(std::set<Item>& items) {
        std::set<Item> tempItems;
        std::queue<Item> closureItems;//转成队列存储
        for (const auto& item : items) {
            closureItems.push(item);
            tempItems.insert(item);
        }
        while (!closureItems.empty())
        {
            const auto& item = closureItems.front();
            items.insert(item);
            int size = item.production.right.size();
            if (item.dotPosition < size)//后面还有符号
            {
                std::string token = item.production.right[item.dotPosition];
                if (isNonTerminal(token))//只有非终结符才需要处理
                {
                    for (const auto& pro : grammar)//遍历每条产生式
                    {
                        if (pro.left != token)
                            continue;
                        std::vector<std::string> tokens;
                        for (int i = item.dotPosition + 1; i < size; i++)
                        {
                            tokens.push_back(item.production.right[i]);
                        }
                        tokens.push_back(item.lookahead);
                        std::set<std::string> terminals = getFirstSetByVector(tokens);//找出First集中所有的终结符号
                        terminals.erase(""); // 删除空串（""）
                        for (const auto& terminal : terminals)
                        {
                            Item ii = Item({ Production{pro.left, pro.right}, 0, terminal });
                            if (items.find(ii) == items.end()&&tempItems.find(ii)==tempItems.end())//之前不存在的才添加进来
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
    /// 计算项目集的转移
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
    /// 生成LR(1)项集族
    /// </summary>
    void genItems() {
        bool added = false;
        itemSets[0] = { Item({ Production{"program", {"block"}}, 0, "$" }) };//初始化program->.block , $
        closure(itemSets[0]);//求闭包

        std::queue<std::set<Item>> lists;
        std::set<std::set<Item>> tempSets;

        lists.push(itemSets[0]);
        tempSets.insert(itemSets[0]);
        int i = 0;
        std::set<std::string> tokens;
        tokens.insert(terminals.begin(), terminals.end());
        tokens.insert(nonTerminals.begin(), nonTerminals.end());//合并终结符和非终结符
        while (!lists.empty())
        {
            std::set<Item> itemSet = lists.front();
            itemSets[i++] = itemSet;
            std::cout << "当前的i的值：" << i << std::endl;
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