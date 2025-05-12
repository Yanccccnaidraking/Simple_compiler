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

	// ���嶯����Ӧ�ĺ�����
	using SemanticAction = std::function<void()>;

	class Parser {
	private:
		Lexer::Lexer& lexer;
		shared_ptr<Lexer::Token> look;
		int stackTop = -1;//ջ���±�
		SmartArray<int> stateStack;
		SmartArray<std::shared_ptr<Inter::Node>> nodeStack;//��stateStackͬ��������������Ϊ�˹�����̱�ʱ�ܷ������ʾ״̬ջ����˲�������ջ�ֱ�洢
		std::shared_ptr<Symbols::Env> top; // ��ǰ�򶥲��ķ��ű�
		int used = 0; // storage used for declarations����������������ڴ�ռ䣩
		std::vector<SemanticAction> semanticActions;//�洢���й�Լ�����嶯��
		std::vector<std::shared_ptr<Symbols::Env>> scopes;//�洢���е������������������
	public:
		Parser(Lexer::Lexer& l);

		//��ʼ�����嶯������
		void initActions();

		//���ö�Ӧ�����嶯��
		void applyAction(int ruleIndex) {
			if (ruleIndex >= 0 && ruleIndex < semanticActions.size()) {
				semanticActions[ruleIndex]();  // �������嶯��
			}
		}

		void showScopes();

		// ��ȡ��һ���ʷ���Ԫ�� look
		void move();

		void error(std::string s);

		void match(int t);

		// program -> block
		void program(); 
	};
}