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
        std::cout << itemSets.size() << std::endl;
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
                        nullable = false;
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

    /// <summary>
    /// 给定某一串产生式的体，计算出对应的First集合
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
            if (isTerminal(item))//若为终结符，则不再向后遍历
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
        itemSets[0] = { Item({ Production{"program", {"block"}}, 0, "$" }) };//初始化program->.block , $
        closure(itemSets[0]);//求闭包

        std::vector<std::set<Item>> arr;
        //std::queue<std::set<Item>> lists;
        std::set<std::set<Item>> tempSets;
        
        arr.push_back(itemSets[0]);

        //lists.push(itemSets[0]);
        tempSets.insert(itemSets[0]);
        int i = 0;
        //std::set<std::string> tokens;
        //tokens.insert(terminals.begin(), terminals.end());
        //tokens.insert(nonTerminals.begin(), nonTerminals.end());//合并终结符和非终结符
        //while (!lists.empty())
        //{
        //    std::set<Item> itemSet = lists.front();
        //    //分成终结和非终结两个部分的原因是需要分别求Action表和GOTO表，不适合合并处理
        //    for (const auto& token : terminals)//终结符号部分
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
        //    for (const auto& token : nonTerminals)//非终结符号部分
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
            //分成终结和非终结两个部分的原因是需要分别求Action表和GOTO表，不适合合并处理

            //编写Action表
            // 由于改文法不是LR(1)文法所以需要进行一定的约束，使其不会产生冲突。
            // 文法存在悬空else的问题，为了解决这一问题，需要在输入为else的时候，选择移入，而不是规约。
            // 由于本文法不存在其他的冲突，所以采取先规约，再移入的填表策略，让移入覆盖规约，以解决冲突

            //规约操作
            for (const auto& item : itemSet)
            {
                if (item.dotPosition == item.production.right.size())//点在最右侧，说明应该规约
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

            for (const auto& token : terminals)//终结符号部分
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
            for (const auto& token : nonTerminals)//非终结符号部分
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