#pragma once
#include "op.h"
#include "id.h"
#include "expr.h"
#include "Symbols/symbols.h"
#include "Lexer/Tag.h"
#include "Lexer/Word.h"
namespace Symbols {
    class Type;
}
namespace Inter
{
    class Access : public Op {
    public:
        std::shared_ptr<Id> array;
        std::shared_ptr<Expr> index;
        bool isArray;//判断是不是数组中的元素

        Access(std::shared_ptr<Id> a, std::shared_ptr<Expr> i, std::shared_ptr<Symbols::Type> p,bool array)
            : array(a), index(i), isArray(array), Op(std::make_shared<Lexer::Word>("[]", Lexer::Tag::INDEX), p) {}

        std::shared_ptr<Expr> gen(){ 
             return std::make_shared<Access>(array, index->reduce(), type, isArray);
        }

        void jumping(int t, int f){ emitjumps(reduce()->toString(), t, f); }

        std::string toString() const { return array->toString() + " [ " + index->toString() + " ]";}
    };
}