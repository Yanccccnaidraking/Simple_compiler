#pragma once
#include "node.h"
#include "Symbols/symbols.h"
#include "Lexer/Token.h" 
#include "Lexer/Word.h"  
#include <memory>

namespace Inter {
	class Expr :public Node
	{
	public:
		std::shared_ptr<Lexer::Token> op;
		std::shared_ptr<Symbols::Type> type;
		bool isConst;
		Expr(std::shared_ptr<Lexer::Token> tok, std::shared_ptr<Symbols::Type>  p, bool isConst = false) : op(tok), type(p),isConst(isConst) {}

		virtual std::shared_ptr<Expr> gen() { return make_shared<Expr>(op,type); }
		virtual std::shared_ptr<Expr> reduce() { return make_shared<Expr>(op,type); }

		virtual void jumping(int t, int f) {
			emitjumps(toString(), t, f);
		}

		/// <summary>
		/// c1.t!=0 && f!=0:
		/// if test goto L<t>
		/// goto L<f>
		/// c2.t!=0 && f==0
		/// if test goto L<t>
		/// c3.t!=0 && f==0
		/// iffalse test goto L<f>
		/// c4.t==0 && f==0
		/// </summary>
		/// <param name="test"></param>
		/// <param name="t"></param>
		/// <param name="f"></param>
		virtual void emitjumps(std::string test, int t, int f) {
			if (t != 0 && f != 0) {
				emit("if " + test + " goto L" + std::to_string(t));
				emit("goto L" + std::to_string(f));
			}
			else if (t != 0) {
				emit("if " + test + " goto L" + std::to_string(t));
			}
			else if (f != 0) {
				emit("iffalse " + test + " goto L" + std::to_string(f));
			}
		} 
		
		virtual std::string toString() const { return op->toString(); }
	};
}