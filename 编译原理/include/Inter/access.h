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
        Id* array;
        Expr* index;

        Access(Id* a, Expr* i, Symbols::Type* p)
            : Op(Lexer::Word("[]", Lexer::Tag::INDEX), p), array(a), index(i) {}

        Expr* gen(){ return new Access(array, &(index->reduce()), type); }

        void jumping(int t, int f){ emitjumps(reduce()->toString(), t, f); }

        std::string toString(){ return array->toString() + " [ " + index->toString() + " ]";}
    };
}

