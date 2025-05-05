#include "Lexer/Word.h"
namespace Lexer {
	// 静态常量 shared_ptr 的定义和初始化
	const std::shared_ptr<Word> Word::and_ = std::make_shared<Word>("&&", Tag::AND);
	const std::shared_ptr<Word> Word::or_ = std::make_shared<Word>("||", Tag::OR);
	const std::shared_ptr<Word> Word::eq = std::make_shared<Word>("==", Tag::EQ);
	const std::shared_ptr<Word> Word::ne = std::make_shared<Word>("!=", Tag::NE);
	const std::shared_ptr<Word> Word::le = std::make_shared<Word>("<=", Tag::LE);
	const std::shared_ptr<Word> Word::ge = std::make_shared<Word>(">=", Tag::GE);
	const std::shared_ptr<Word> Word::minus = std::make_shared<Word>("minus", Tag::MINUS);
	const std::shared_ptr<Word> Word::true_ = std::make_shared<Word>("true", Tag::TRUE);
	const std::shared_ptr<Word> Word::false_ = std::make_shared<Word>("false", Tag::FALSE);
	const std::shared_ptr<Word> Word::temp = std::make_shared<Word>("temp", Tag::TEMP);
}