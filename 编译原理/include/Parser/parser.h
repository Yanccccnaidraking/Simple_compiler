#pragma once
#include "Lexer/lexer.h"
#include "Symbols/symbols.h"
#include "Inter/stmt.h"
#include "Inter/node.h"
#include "Parser/lr_1.h"
#include<stack>
#include <functional> 

namespace Lexer {
	class Lexer;
}

namespace Symbols {
	class Type;
}

namespace Inter {
	class Stmt;
	class Expr;
	class Op;
	class Access;
	class Id;
	class Node;
}

namespace Parser {

	// 语义动作对应的函数：
	using SemanticAction = std::function<void()>;

	class Parser {
	private:
		Lexer::Lexer& lexer;
		shared_ptr<Lexer::Token> look;
		int stackTop = -1;//栈顶下标
		SmartArray<int> stateStack;
		SmartArray<std::shared_ptr<Inter::Node>> nodeStack;//与stateStack同步，共进共出，为了构造过程表时能方便的显示状态栈，因此采用两个栈分别存储
		std::shared_ptr<Symbols::Env> top; // 当前或顶部的符号表
		int used = 0; // storage used for declarations（声明变量所需的内存空间）
		std::vector<SemanticAction> semanticActions;//存储所有规约的语义动作
		std::vector<std::shared_ptr<Symbols::Env>> scopes;//存储所有的作用域，用于最后的输出
	public:
		Parser(Lexer::Lexer& l);

		//初始化语义动作函数
		void initActions();

		//调用对应的语义动作
		void applyAction(int ruleIndex) {
			if (ruleIndex >= 0 && ruleIndex < semanticActions.size()) {
				semanticActions[ruleIndex]();  // 调用语义动作
			}
		}

		void showScopes();

		// 获取下一个词法单元到 look
		void move();

		void error(std::string s);

		void match(int t);

		// program -> block
		void program(); 
	};
}