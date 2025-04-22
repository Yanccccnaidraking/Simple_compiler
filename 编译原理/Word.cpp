#include "Word.h"
namespace Lexer {
    const Word Word::and_("&&", Tag::AND);
    const Word Word::or_("||", Tag::OR);
    const Word Word::eq("==", Tag::EQ);
    const Word Word::ne("!=", Tag::NE);
    const Word Word::le("<=", Tag::LE);
    const Word Word::ge(">=", Tag::GE);
    const Word Word::minus("minus", Tag::MINUS);
    const Word Word::true_("true", Tag::TRUE);
    const Word Word::false_("false", Tag::FALSE);
    const Word Word::temp("temp", Tag::TEMP);
}