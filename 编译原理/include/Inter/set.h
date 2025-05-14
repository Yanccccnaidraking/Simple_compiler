#pragma once
#include "stmt.h"
#include "id.h"
#include "Symbols/symbols.h"

namespace Inter
{
    class Set : public Stmt {
    public:

        std::shared_ptr<Id> id;

        std::shared_ptr<Expr> expr;


        Set(std::shared_ptr<Id> i, std::shared_ptr<Expr> x) : id(i), expr(x) {
            if (check(id->type, expr->type) == nullptr) {
                error("type error");
            }
        }

        std::shared_ptr<Symbols::Type> check(std::shared_ptr<Symbols::Type> p1, std::shared_ptr<Symbols::Type> p2) {
            if (Symbols::Type::numeric(p1) && Symbols::Type::numeric(p2)) {
                return p2;
            }
            else if (p1 == Symbols::Type::Bool && p2 == Symbols::Type::Bool) {
                return p2;
            }
            return nullptr;
        }
        void gen(int b, int a) {
            std::string code = id->toString() + " = " + (*expr->gen()).toString();
            emit(code);
        }

        void error(const std::string& msg) {
            // 错误处理实现
            throw std::runtime_error("Set Error:" + msg);
        }
    };
}