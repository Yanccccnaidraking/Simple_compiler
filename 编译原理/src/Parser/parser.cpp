#include "Parser/parser.h"
#include "Lexer/lexer.h"
#include "Lexer/Tag.h"
#include "Lexer/Num.h"
#include "Parser/lr_1.h"
#include "Inter/terminalnode.h"
#include "Inter/typenode.h"
#include "Symbols/symbols.h"
#include "Inter/id.h"
#include "Inter/seq.h"
#include "Inter/access.h"
#include "Inter/setElem.h"
#include "Inter/set.h"
#include "Inter/if.h"
#include "Inter/else.h"
#include "Inter/while.h"
#include "Inter/do.h"
#include "Inter/break.h"
#include "Inter/constant.h"
#include "Inter/arith.h"
#include "Inter/or.h"
#include "Inter/and.h"
#include "Inter/rel.h"
#include "Inter/unary.h"
#include "Inter/not.h"
#include "Inter/continue.h"
#include "Inter/case.h"
#include "Inter/switch.h"
#include <stack>
namespace Parser {
	Parser::Parser(Lexer::Lexer& l) : lexer(l), top(nullptr)
	{
		//deserializeItemSets("ItemSets.dat");
		//printItemSets();
		/*computeFirstSets();
		printFirstSets();*/
		loadGotoTable("GOTOTable.dat");
		loadActionTable("ActionTable.dat");
		//printActionTable();
		//printGotoTable();
		//writeActionTableToFile("./lr_1_tab.txt");
		top = std::make_shared<Symbols::Env>(nullptr);
		initActions();
		stateStack[++stackTop] = 0;
		nodeStack[stackTop] = nullptr;
		move();
	}

	/// <summary>
	/// 输出符号表信息
	/// </summary>
	void Parser::showScopes()
	{
		std::cout << "\n符号表：\n";
		// 表头
		std::cout << "| 作用域 ID |     变量名     | 类型       | 地址偏移量 |   大小(Byte)   |\n";
		std::cout << "|-----------|----------------|------------|------------|----------------|\n";
		int size = scopes.size();
		for (int i = 0; i < size; i++)
		{
			auto table = scopes[i]->getTable();
			for (auto& tt : table)
			{
				std::cout << "| "
					<< std::setw(9) << i << " | "
					<< std::setw(14) << tt.first << " | "
					<< std::setw(10) << tt.second->type->toString() << " | "
					<< std::setw(10) << tt.second->offset << " | "
					<< std::setw(14) << tt.second->type->width << " |\n";
			}
		}
	}

	void Parser::initActions()
	{
		semanticActions = {
			[this]() { //program-> block
				std::shared_ptr<Inter::Stmt> s = std::dynamic_pointer_cast<Inter::Stmt>(nodeStack[stackTop]);
				if (s) {
					int begin = s->newlabel(); int after = s->newlabel();
					s->emitlabel(begin); s->gen(begin, after); s->emitlabel(after);
				}
				else {
					error("语句块解析异常");
				}
			},
			[this]() {//block->{decls stmts} 
				std::shared_ptr<Inter::Stmt> s = std::dynamic_pointer_cast<Inter::Stmt>(nodeStack[stackTop - 1]);
				stackTop = stackTop - 3;
				nodeStack[stackTop] = s;
				top = Symbols::Env::exit(top);
			},
			[this]() {//decls->decls decl
				stackTop--;
			},
			[this]() {//decls->ε
				nodeStack[++stackTop] = nullptr;//不参与中间代码生成，仅用于与状态栈对齐
			},
			[this]() {//decl->type id;
					std::shared_ptr<Inter::TerminalNode> idNode = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
					std::shared_ptr<Inter::TypeNode> typeNode = std::dynamic_pointer_cast<Inter::TypeNode>(nodeStack[stackTop - 2]);
					auto type = typeNode->type;
					if (std::dynamic_pointer_cast<Symbols::Array>(type))
					{
						std::stack<int> sizeStack;
						auto temp = type;
						while (temp->tag != (int)(Lexer::Tag::BASIC))
						{
							sizeStack.push((std::dynamic_pointer_cast<Symbols::Array>(temp))->size);
							temp = (std::dynamic_pointer_cast<Symbols::Array>(temp))->of;
						}
						(std::dynamic_pointer_cast<Symbols::Array>(type))->setWidth(sizeStack);
					}
					auto word = std::dynamic_pointer_cast<Lexer::Word>(idNode->token);
					std::shared_ptr<Inter::Id> id = std::make_shared<Inter::Id>(word, typeNode->type, used);
					top->put(idNode->token->toString(), id);
					used += typeNode->type->width;
					stackTop -= 2;
			},
			[this]() {//type->type[num]
				auto terNode = std::dynamic_pointer_cast<Inter::TypeNode>(nodeStack[stackTop - 3]);
				auto t = terNode->type;
				auto terNumNode = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
				int size = (std::dynamic_pointer_cast<Lexer::Num>(terNumNode->token))->value;
				auto type = std::make_shared<Symbols::Array>(size,t);
				auto typeNode = std::make_shared<Inter::TypeNode>(type);
				stackTop -= 3;
				nodeStack[stackTop] = typeNode;
			},
			[this]() {//type->basic
				std::shared_ptr<Inter::TerminalNode> terNode = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop]);
				std::shared_ptr<Symbols::Type> t = std::dynamic_pointer_cast<Symbols::Type>(terNode->token);
				std::shared_ptr<Inter::TypeNode> typeNode = std::make_shared<Inter::TypeNode>(t);
				nodeStack[stackTop] = typeNode;
			},
			[this]() {//stmts->stmts stmt
				auto stmt = std::dynamic_pointer_cast<Inter::Stmt>(nodeStack[stackTop]);
				auto stmts = std::dynamic_pointer_cast<Inter::Stmt>(nodeStack[stackTop - 1]);
				auto curStmts = std::make_shared<Inter::Seq>(stmts, stmt);
				stackTop--;
				nodeStack[stackTop] = curStmts;
			},
			[this]() {//stmts->ε
				std::shared_ptr<Inter::Stmt> stmts = Inter::Stmt::Null;
				nodeStack[++stackTop] = stmts;
			},
			[this]() {//stmt->loc=bool;
				auto loc = std::dynamic_pointer_cast<Inter::Access>(nodeStack[stackTop - 3]);
				auto boolNode = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 1]);
				stackTop -= 3;				
				if (loc->isArray) {
					auto stmt = std::make_shared<Inter::SetElem>(loc, boolNode);
					nodeStack[stackTop] = stmt;
				}
				else {
					auto stmt = std::make_shared<Inter::Set>(loc->array, boolNode);
					nodeStack[stackTop] = stmt;
				}				
			},
			[this]() {//stmt->if (bool) stmt
				auto stmt = std::dynamic_pointer_cast<Inter::Stmt>(nodeStack[stackTop]);
				auto boolNode = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto curStmt = std::make_shared<Inter::If>(boolNode, stmt);
				stackTop -= 4;
				nodeStack[stackTop] = curStmt;
			},
			[this]() {//stmt->if (bool) stmt else stmt
				auto stmt1 = std::dynamic_pointer_cast<Inter::Stmt>(nodeStack[stackTop - 2]);
				auto stmt2 = std::dynamic_pointer_cast<Inter::Stmt>(nodeStack[stackTop]);
				auto boolNode = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 4]);
				stackTop -= 6;
				auto curStmt = std::make_shared<Inter::Else>(boolNode, stmt1, stmt2);
				nodeStack[stackTop] = curStmt;
			},
			[this]() {//stmt->while (bool) stmt
				auto stmt = std::dynamic_pointer_cast<Inter::Stmt>(nodeStack[stackTop]);
				auto boolNode = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto curStmt = std::make_shared<Inter::While>();
				curStmt->init(boolNode, stmt);
				stackTop -= 4;
				nodeStack[stackTop] = curStmt;
			},
			[this]() {//stmt->do stmt while (bool);
				auto stmt = std::dynamic_pointer_cast<Inter::Stmt>(nodeStack[stackTop - 5]);
				auto boolNode = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto curStmt = std::make_shared<Inter::Do>();
				curStmt->init(stmt, boolNode);
				stackTop -= 6;
				nodeStack[stackTop] = curStmt;
			},
			[this]() {//stmt->break;
				auto curStmt = std::make_shared<Inter::Break>();
				stackTop--;
				nodeStack[stackTop] = curStmt;
			},
			[this]() {//stmt->continue;
				auto curStmt = std::make_shared<Inter::Continue>();
				stackTop--;
				nodeStack[stackTop] = curStmt;
			},
			[this]() {//stmt->block
				//不需要任何操作
			},
			[this]() {//loc->loc[bool]
				auto boolNode = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 1]);
				auto loc = std::dynamic_pointer_cast<Inter::Access>(nodeStack[stackTop - 3]);

				std::shared_ptr<Symbols::Type> originType(loc->type);
				std::shared_ptr<Symbols::Type> type = originType;
				std::shared_ptr<Inter::Expr> w, t1, t2, loc1;
				type = std::dynamic_pointer_cast<Symbols::Array>(type);
				if (!type) {
					throw std::runtime_error("访问非数组类型");
				}
				type = std::dynamic_pointer_cast<Symbols::Array>(type)->of;
				w = std::make_shared<Inter::Constant>(type->width);
				t1 = std::make_shared<Inter::Arith>(std::make_shared<Lexer::Token>('*'), boolNode, w);
				if (loc->isArray) {
					t2 = std::make_shared<Inter::Arith>(std::make_shared<Lexer::Token>('+'), loc->index, t1);
					loc1 = t2;
				}
				else {
					loc1 = t1;
				}
				auto curLoc = std::make_shared<Inter::Access>(loc->array, loc1, type, true);
				stackTop -= 3;
				nodeStack[stackTop] = curLoc;

				
			},
			[this]() {//loc->id
				auto idNode = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop]);
				auto id = top->get(idNode->token->toString());
				if (id == nullptr) {
					throw std::runtime_error("未声明的变量:" + idNode->token->toString());
				}
				auto curLoc = std::make_shared<Inter::Access>(id, nullptr,id->type, false);
				nodeStack[stackTop] = curLoc;			
			},
			[this]() {//bool->bool || join
				auto join = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop]);
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
				auto boolNode = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto curBool = std::make_shared<Inter::Or>(token->token, boolNode, join);
				stackTop -= 2;
				nodeStack[stackTop] = curBool;
			},
			[this]() {//bool->join
				//不需要任何操作
			},
			[this]() {//join->join && equality
				auto e = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop]);
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
				auto join = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto curJoin = std::make_shared<Inter::And>(token->token, join, e);
				stackTop -= 2;
				nodeStack[stackTop] = curJoin;
			},
			[this]() {//join->equality
				//不需要任何操作
			},
			[this]() {//equality->equality==rel
				auto rel = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop]);
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
				auto e = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto curE = std::make_shared<Inter::Rel>(token->token, e, rel);
				stackTop -= 2;
				nodeStack[stackTop] = curE;
			},
			[this]() {//equality->equality!=rel
				auto rel = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop]);
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
				auto e = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto curE = std::make_shared<Inter::Rel>(token->token, e, rel);
				stackTop -= 2;
				nodeStack[stackTop] = curE;
			},
			[this]() {//equality->rel
				//不需要任何操作
			},
			[this]() {//rel->expr<expr
				auto expr2 = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop]);
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
				auto expr1 = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto rel = std::make_shared<Inter::Rel>(token->token, expr1, expr2);
				stackTop -= 2;
				nodeStack[stackTop] = rel;
			},
			[this]() {//rel->expr<=expr
				auto expr2 = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop]);
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
				auto expr1 = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto rel = std::make_shared<Inter::Rel>(token->token, expr1, expr2);
				stackTop -= 2;
				nodeStack[stackTop] = rel;
			},
			[this]() {//rel->expr>=expr
				auto expr2 = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop]);
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
				auto expr1 = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto rel = std::make_shared<Inter::Rel>(token->token, expr1, expr2);
				stackTop -= 2;
				nodeStack[stackTop] = rel;
			},
			[this]() {//rel->expr>expr
				auto expr2 = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop]);
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
				auto expr1 = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto rel = std::make_shared<Inter::Rel>(token->token, expr1, expr2);
				stackTop -= 2;
				nodeStack[stackTop] = rel;
			},
			[this]() {//rel->expr
				//不需要任何操作
			},
			[this]() {//expr->expr+term
				auto term = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop]);
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
				auto expr1 = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto expr = std::make_shared<Inter::Arith>(token->token, expr1, term);
				stackTop -= 2;
				nodeStack[stackTop] = expr;
			},
			[this]() {//expr->expr-term
				auto term = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop]);
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
				auto expr1 = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto expr = std::make_shared<Inter::Arith>(token->token, expr1, term);
				stackTop -= 2;
				nodeStack[stackTop] = expr;
			},
			[this]() {//expr->term
				//不需要任何操作
			},
			[this]() {//term->term*unary
				auto unary = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop]);
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
				auto term1 = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto term = std::make_shared<Inter::Arith>(token->token, term1, unary);
				stackTop -= 2;
				nodeStack[stackTop] = term;
			},
			[this]() {//term->term/unary
				auto unary = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop]);
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
				auto term1 = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				auto term = std::make_shared<Inter::Arith>(token->token, term1, unary);
				stackTop -= 2;
				nodeStack[stackTop] = term;
			},
			[this]() {//term->unary
				//不需要任何操作
			},
			[this]() {//unary-> ! unary
				auto unary1 = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop]);
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop - 1]);
				auto unary = std::make_shared<Inter::Not>(token->token, unary1);
				stackTop--;
				nodeStack[stackTop] = unary;
			},
			[this]() {//unary-> - unary
				auto unary1 = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop]);
				auto unary = std::make_shared<Inter::Unary>(Lexer::Word::minus, unary1);
				stackTop--;
				nodeStack[stackTop] = unary;
			},
			[this]() {//unary-> factor
				//不需要任何操作
			},
			[this]() {//factor->(bool)
				auto boolNode = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 1]);
				stackTop -= 2;
				nodeStack[stackTop] = boolNode;
			},
			[this]() {//factor->loc
				auto node = std::dynamic_pointer_cast<Inter::Access>(nodeStack[stackTop]);
				if (!(node->index))
					nodeStack[stackTop] = node->array;
			},
			[this]() {//factor->num
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop]);
				nodeStack[stackTop] = std::make_shared<Inter::Constant>(token->token, Symbols::Type::Int);
			},
			[this]() {//factor->real
				auto token = std::dynamic_pointer_cast<Inter::TerminalNode>(nodeStack[stackTop]);
				nodeStack[stackTop] = std::make_shared<Inter::Constant>(token->token, Symbols::Type::Float);
			},
			[this]() {//factor->true
				nodeStack[stackTop] = Inter::Constant::True;
			},
			[this]() {//factor->false
				nodeStack[stackTop] = Inter::Constant::False;
			},
			[this]() {//stmt->switch(expr){case_list default_opt}
				auto list = std::dynamic_pointer_cast<Inter::CaseList>(nodeStack[stackTop - 2]);
				auto caseNode = std::dynamic_pointer_cast<Inter::Case>(nodeStack[stackTop-1]);
				auto expr = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 5]);
				if (expr&&expr->type!=Symbols::Type::Int)
				{
					error("switch语句必须是Int型");
				}
				auto swit = std::make_shared<Inter::Switch>(expr, list, caseNode);
				stackTop -= 7;
				nodeStack[stackTop] = swit;
			},
			[this]() {//case_list->case_list case_stmt
				auto list = std::dynamic_pointer_cast<Inter::CaseList>(nodeStack[stackTop-1]);
				auto caseNode = std::dynamic_pointer_cast<Inter::Case>(nodeStack[stackTop]);
				list->addCase(caseNode);
				stackTop--;
			},
			[this]() {//case_list->ε
				auto list = std::make_shared<Inter::CaseList>();
				stackTop++;
				nodeStack[stackTop] = list;
			},
			[this]() {//case_stmt->case expr : stmts
				auto stmt = std::dynamic_pointer_cast<Inter::Stmt>(nodeStack[stackTop]);
				auto expr = std::dynamic_pointer_cast<Inter::Expr>(nodeStack[stackTop - 2]);
				if (!expr||!expr->isConst)
				{
					error("case语句必须仅包含常量");
				}
				auto caseNode = std::make_shared<Inter::Case>(expr, stmt);
				stackTop -= 3;
				nodeStack[stackTop] = caseNode;
			},
			[this]() {//default_opt->default : stmt
				auto stmt = std::dynamic_pointer_cast<Inter::Stmt>(nodeStack[stackTop]);
				auto caseNode = std::make_shared<Inter::Case>(nullptr, stmt);
				stackTop -= 2;
				nodeStack[stackTop] = caseNode;
			},
			[this]() {//default_opt->ε
				nodeStack[++stackTop] = nullptr;
			},
		};
	}

	void Parser::move()
	{
		look = lexer.scan();
	}
	void Parser::error(std::string s)
	{
		throw runtime_error("line " + std::to_string(lexer.line) + ": " + s);
	}
	void Parser::match(int t)
	{
		if (look->tag == t) move();
		else error("语法错误");
	}
	void Parser::program()
	{
		bool finished = false;
		std::deque<std::string> symbols;
		std::string token = Lexer::tagToString(Lexer::Tag(look->tag));
		try{
			while (!finished)
			{
				int cur = stateStack[stackTop];
				//cout << "栈顶的数值是：" << cur << endl;
				//cout<<token<<endl;
				if (cur >= 0)
				{
					Action act = searchFromAction(cur, token);
					switch (act.type)
					{
					case ActionType::Shift:
						insertParserTable(stateStack, symbols, token, "移入");
						if (token == "{") {
							top = std::make_shared<Symbols::Env>(top);//进入新的语句块，作用域发生切换
							scopes.push_back(top);
						}
						stateStack[++stackTop] = act.value;
						nodeStack[stackTop] = std::make_shared<Inter::TerminalNode>(look);
						move();
						symbols.push_back(token);
						token = Lexer::tagToString(Lexer::Tag(look->tag));
						break;
					case ActionType::Reduce:
					{
						try {
							insertParserTable(stateStack, symbols, token, "根据" + toStringProduction(grammar[act.value]) + "规约");
							applyAction(act.value);
							int newState = searchFromGoto(stateStack[stackTop - 1], grammar[act.value].left);
							stateStack[stackTop] = newState;
							stateStack.setMaxSize(stackTop + 1);
							symbols.push_back(grammar[act.value].left);
							break;
						}
						catch (const std::exception& e) {
							std::cerr << "在规约时发生错误: " << e.what() << std::endl;
						}						
					}
					case ActionType::Accept:
						insertParserTable(stateStack, symbols, token, "接收");
						applyAction(0);
						finished = true;//语法分析结束
						break;
					case ActionType::Error:
						//调用错误报告例程
						error("语法分析错误");
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		if (lexer.error_info != "")
		{
			cout <<"\033[31m"<< "\n词法分析错误详情：" << endl;
			cout << lexer.error_info << "\033[0m" << endl;
		}
	}
}