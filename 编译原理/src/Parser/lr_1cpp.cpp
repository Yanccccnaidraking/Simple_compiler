#include "Parser/lr_1.h"
#include<queue>
namespace Parser {

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
    std::set<std::string> terminals = { "[","]","{", "}", ";", "(", ")", "+", "-", "*", "/", "!","=","==", "!=", "<", "<=", ">", ">=", "||", "&&", "$","num", "real", "true", "false","if","while","else","do","break","id","basic","" };
    std::set<std::string> nonTerminals = { "program", "block", "decls", "decl", "type", "stmts", "stmt", "loc", "bool", "join", "equality", "rel", "expr", "term", "unary", "factor" };
    std::map<std::string, std::set<std::string>> firstSets;
    std::unordered_map<int, std::unordered_map<std::string, int>> actionTable; // Action Table
    std::unordered_map<int, std::unordered_map<std::string, int>> gotoTable; // Goto Table
    std::unordered_map<int, std::set<Item>> itemSets; // 项集族

    /// <summary>
    /// 查询LR(1)分析表的Action
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
    /// 查询LR(1)分析表的GOTO
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
                        actionTable[j][item.lookahead] = encodeAction(ActionType::Reduce, proNo);
                    }
                    else if (!proNo)
                    {
                        actionTable[j][item.lookahead] = encodeAction(ActionType::Accept,0);
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

    /// <summary>
    /// 解码分析表的符号
    /// </summary>
    /// <param name="type"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    int encodeAction(ActionType type, int value) {
        return (static_cast<int>(type) << 28) | (value & 0x0FFFFFFF);
    }

    /// <summary>
    /// 编码分析表的action
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