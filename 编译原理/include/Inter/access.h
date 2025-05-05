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

        Access(std::shared_ptr<Id> a, std::shared_ptr<Expr> i, Symbols::Type* p)
            : array(a), index(i), Op(Lexer::Word("[]", Lexer::Tag::INDEX), p) {}

        std::shared_ptr<Expr> gen(){ 
            return std::make_shared<Access>(array, index->reduce(), type);
        }

        void jumping(int t, int f){ emitjumps(reduce()->toString(), t, f); }

        std::string toString(){ return array->toString() + " [ " + index->toString() + " ]";}
    };
}

