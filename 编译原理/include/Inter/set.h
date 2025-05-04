#pragma once
#include "stmt.h"
#include "id.h"
#include "Symbols/symbols.h"

namespace Inter
{
    class Set : public Stmt {
    public:
        Id* id;
        Expr* expr;

        Set(Id* i, Expr* x) : id(i), expr(x) {
            if (check(id->type, expr->type) == nullptr) {
                error("type error");
            }
        }

        Symbols::Type* check(Symbols::Type* p1, Symbols::Type* p2) {
            if (Symbols::Type::typePriority(p1) && Symbols::Type::typePriority(p2)) {
                return p2;
            }
            else if (p1 == Symbols::Type::Bool && p2 == Symbols::Type::Bool) {
                return p2;
            }
            return nullptr;
        }
        void gen(int b, int a) {
            std::string code = id->toString() + " = " + expr->gen().toString();
            emit(code);
        }

        void error(const std::string& msg) {
            // 错误处理实现
            throw std::runtime_error(msg);
        }
    };
}