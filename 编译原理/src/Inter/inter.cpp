#include "Lexer/Word.h"
#include "Symbols/symbols.h"

#include "Inter/node.h"
#include "Inter/while.h"
#include "Inter/stmt.h"
#include "Inter/constant.h"
#include "Inter/temp.h"

#include <stdexcept>


namespace Inter
{
	int Inter::Node::labels = 1;
	std::shared_ptr<Stmt> Stmt::Null = std::make_shared<Stmt>();
	int Temp::count = 0;
}