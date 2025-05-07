#pragma once

#include "stmt.h"
#include "id.h"
#include "access.h"
#include "Symbols/symbols.h"

namespace Inter
{
    class SetElem : public Stmt {
    public:

        std::shared_ptr<Id> array;

        std::shared_ptr<Expr> index;

        std::shared_ptr<Expr> expr;


        SetElem(std::shared_ptr<Access> x, std::shared_ptr<Expr> y)
            : array(x->array), index(x->index), expr(y) {
            if (check(x->type, expr->type) == nullptr) {
                error("type error");
            }
        }

        std::shared_ptr<Symbols::Type> check(std::shared_ptr<Symbols::Type> p1, std::shared_ptr<Symbols::Type> p2)
        {
            // 检查数组类型
            if (dynamic_pointer_cast<Symbols::Array>(p1) || dynamic_pointer_cast<Symbols::Array>(p2)) {
                return nullptr;
            }
            // 检查类型完全匹配
            else if (p1 == p2) {
                return p2;
            }
            // 检查数值类型兼容
            else if (Symbols::Type::typePriority(p1) && Symbols::Type::typePriority(p2)) {
                return p2;
            }
            return nullptr;
        }
        void gen(int b, int a) {
            std::string s1 = (*index->reduce()).toString();
            std::string s2 = (*expr->reduce()).toString();
            std::string code = array->toString() + " [ " + s1 + " ] = " + s2;
            emit(code);
        }

    private:
        void error(const std::string& msg) {
            throw std::runtime_error("SetElem: " + msg);
        }
    };
}