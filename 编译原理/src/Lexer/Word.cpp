#include "Lexer/Word.h"
namespace Lexer {
	// 静态常量 shared_ptr 的定义和初始化
	  std::shared_ptr<Word> Word::and_ = std::make_shared<Word>("&&", Tag::AND);
	  std::shared_ptr<Word> Word::or_ = std::make_shared<Word>("||", Tag::OR);
	  std::shared_ptr<Word> Word::eq = std::make_shared<Word>("==", Tag::EQ);
	  std::shared_ptr<Word> Word::ne = std::make_shared<Word>("!=", Tag::NE);
	  std::shared_ptr<Word> Word::le = std::make_shared<Word>("<=", Tag::LE);
	  std::shared_ptr<Word> Word::ge = std::make_shared<Word>(">=", Tag::GE);
	  std::shared_ptr<Word> Word::minus = std::make_shared<Word>("minus", Tag::MINUS);
	  std::shared_ptr<Word> Word::true_ = std::make_shared<Word>("true", Tag::TRUE);
	  std::shared_ptr<Word> Word::false_ = std::make_shared<Word>("false", Tag::FALSE);
	  std::shared_ptr<Word> Word::temp = std::make_shared<Word>("temp", Tag::TEMP);
}