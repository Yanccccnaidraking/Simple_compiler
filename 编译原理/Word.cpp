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
    const Word Word::while_("while", Tag::WHILE);
    const Word Word::temp("temp", Tag::TEMP);
    const Word Word::if_("if", Tag::IF);
    const Word Word::for_("for", Tag::FOR);
    const Word Word::else_("else", Tag::ELSE);
    const Word Word::switch_("switch", Tag::SWITCH);
    const Word Word::case_("case", Tag::CASE);
    const Word Word::default_("default", Tag::DEFAULT);
}