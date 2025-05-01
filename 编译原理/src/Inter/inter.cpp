#include "Lexer/Word.h"
#include "Symbols/symbols.h"

#include "Inter/node.h"
#include "Inter/while.h"


#include <stdexcept>


namespace Inter
{

	//while�ľ���ʵ��
	While::While() : expr(nullptr), stmt(nullptr), after(0) {}
    
    void While::init(Expr* x, Stmt* s) {
        expr = x;
        stmt = s;
        if (expr->type != Symbols::Type::Bool) {
            error("boolean required in while");
        }
    }

    void While::gen(int b, int a) {
        after = a;
        expr->jumping(0, a);

        int label = newlabel();
        emitlabel(label);
        stmt->gen(label, b);
        emit("goto L" + std::to_string(b));  // ѭ�����������ؿ�ͷ
    }

    void While::error(const std::string& msg) {
        throw std::runtime_error("While: " + msg);
    }
	
}