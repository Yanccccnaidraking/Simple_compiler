#pragma once
#include "Lexer/lexer.h"
#include "Symbols/symbols.h"
#include "Inter/stmt.h"
#include "Inter/node.h"
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

	/// <summary>
	/// 智能数组，能够实现自动扩容
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class SmartArray {
	private:
		std::vector<T> data;
	public:
		T& operator[](size_t index) {
			if (index >= data.size()) {
				data.resize(index + 50);  // 自动扩容
			}
			return data[index];
		}

		const T& operator[](size_t index) const {
			return data[index];
		}

		size_t size() const {
			return data.size();
		}

		void print() const {
			for (const auto& val : data) {
				std::cout << val << " ";
			}
			std::cout << "\n";
		}

		std::vector<T> getVector() {
			return data;
		}
	};

	class Parser {
	private:
		Lexer::Lexer& lexer;
		shared_ptr<Lexer::Token> look;
		int stackTop = -1;//栈顶下标
		SmartArray<int> stateStack;
		SmartArray<std::shared_ptr<Inter::Node>> nodeStack;//与stateStack同步，共进共出，为了构造过程表时能方便的显示状态栈，因此采用两个栈分别存储
		std::shared_ptr<Symbols::Env> top=nullptr; // 当前或顶部的符号表
		int used = 0; // storage used for declarations
		std::vector<SemanticAction> semanticActions;
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

		// 获取下一个词法单元到 look
		void move();

		void error(std::string s);

		void match(int t);

		// program -> block
		void program(); 

		// block -> { decls stmts }
		shared_ptr<Inter::Stmt> block();

		void decls();

		shared_ptr<Symbols::Type> type();

		shared_ptr<Symbols::Type> dims(std::shared_ptr<Symbols::Type> p);

		shared_ptr<Inter::Stmt> stmts();

		shared_ptr<Inter::Stmt> stmt();

		shared_ptr<Inter::Stmt> assign();

		shared_ptr<Inter::Expr> bool_();

		shared_ptr<Inter::Expr> join();

		shared_ptr<Inter::Expr> equality();

		shared_ptr<Inter::Expr> rel();

		shared_ptr<Inter::Expr> expr();

		shared_ptr<Inter::Expr> term();

		shared_ptr<Inter::Expr> unary();

		shared_ptr<Inter::Expr> factor();

		shared_ptr<Inter::Access> offset(shared_ptr<Inter::Id> a);
	};
}