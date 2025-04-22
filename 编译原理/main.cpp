#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

// === 基本配置 ===
const int BUFFER_SIZE = 2048;
char buffer[BUFFER_SIZE];
int lexemeBegin = 0;
int forwardptr = 0;
int bufferEnd = 0;

enum TokenType {
    KEYWORD = 1,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    DELIMITER,
    COMMENT,
    CHAR_CONST,
    STRING_CONST,
    ERROR
};

struct Token {
    TokenType type;
    string lexeme;
};

unordered_set<string> keywords = {
    "int", "float", "if", "else", "for", "while", "return", "void", "char"
};

unordered_set<char> singleOperators = { '+', '-', '*', '/', '=', '>', '<', '!', '&', '|', '%' };
unordered_set<string> doubleOperators = { "==", "!=", ">=", "<=", "&&", "||", "++", "--" };

unordered_set<char> delimiters = { '(', ')', '{', '}', '[', ']', ';', ',' };

vector<string> symbolTable;
unordered_set<string> symbolSet;

// === 工具函数 ===
bool isKeyword(const string& word) {
    return keywords.count(word);
}

bool isDelimiter(char c) {
    return delimiters.count(c);
}

bool isOperator(char c) {
    return singleOperators.count(c);
}

void addToSymbolTable(const string& sym) {
    if (symbolSet.insert(sym).second) {
        symbolTable.push_back(sym);
    }
}

string tokenTypeToStr(TokenType type) {
    switch (type) {
    case KEYWORD: return "KEYWORD";
    case IDENTIFIER: return "IDENTIFIER";
    case NUMBER: return "NUMBER";
    case OPERATOR: return "OPERATOR";
    case DELIMITER: return "DELIMITER";
    case COMMENT: return "COMMENT";
    case CHAR_CONST: return "CHAR_CONST";
    case STRING_CONST: return "STRING_CONST";
    case ERROR: return "ERROR";
    }
    return "UNKNOWN";
}

// === 跳过空白与注释 ===
void skipWhitespaceAndComments() {
    while (forwardptr < bufferEnd) {
        if (isspace(buffer[forwardptr])) {
            forwardptr++;
        }
        else if (buffer[forwardptr] == '/' && forwardptr + 1 < bufferEnd) {
            if (buffer[forwardptr + 1] == '/') {
                forwardptr += 2;
                while (forwardptr < bufferEnd && buffer[forwardptr] != '\n') forwardptr++;
            }
            else if (buffer[forwardptr + 1] == '*') {
                forwardptr += 2;
                while (forwardptr < bufferEnd - 1 && !(buffer[forwardptr] == '*' && buffer[forwardptr + 1] == '/'))
                    forwardptr++;
                if (forwardptr < bufferEnd - 1) forwardptr += 2;
                else cerr << "Unterminated comment!" << endl;
            }
            else {
                break;
            }
        }
        else {
            break;
        }
    }
}

// === Token 识别模块 ===
Token handleIdentifierOrKeyword() {
    while (forwardptr < bufferEnd && (isalnum(buffer[forwardptr]) || buffer[forwardptr] == '_'))
        forwardptr++;
    string lexeme(buffer + lexemeBegin, buffer + forwardptr);
    if (isKeyword(lexeme)) return { KEYWORD, lexeme };
    addToSymbolTable(lexeme);
    return { IDENTIFIER, lexeme };
}

Token handleNumber() {
    bool hasDot = false;
    while (forwardptr < bufferEnd && (isdigit(buffer[forwardptr]) || (!hasDot && buffer[forwardptr] == '.'))) {
        if (buffer[forwardptr] == '.') hasDot = true;
        forwardptr++;
    }
    string number(buffer + lexemeBegin, buffer + forwardptr);
    return { NUMBER, number };
}

Token handleOperator() {
    string op(1, buffer[forwardptr]);
    if (forwardptr + 1 < bufferEnd) {
        string combined = op + buffer[forwardptr + 1];
        if (doubleOperators.count(combined)) {
            forwardptr += 2;
            return { OPERATOR, combined };
        }
    }
    forwardptr++;
    return { OPERATOR, op };
}

Token handleDelimiter() {
    string delim(1, buffer[forwardptr++]);
    return { DELIMITER, delim };
}

Token handleCharConst() {
    forwardptr++;  // skip opening '
    if (forwardptr + 1 < bufferEnd && buffer[forwardptr + 1] == '\'') {
        string val = string(1, buffer[forwardptr]);
        forwardptr += 2; // skip char and closing '
        return { CHAR_CONST, "'" + val + "'" };
    }
    forwardptr++;
    return { ERROR, "Invalid char constant" };
}

Token handleStringConst() {
    forwardptr++; // skip opening "
    int start = forwardptr;
    while (forwardptr < bufferEnd && buffer[forwardptr] != '"') forwardptr++;
    if (forwardptr < bufferEnd) {
        string str(buffer + start, buffer + forwardptr);
        forwardptr++; // skip closing "
        return { STRING_CONST, "\"" + str + "\"" };
    }
    return { ERROR, "Unterminated string" };
}

Token handleError() {
    string err(1, buffer[forwardptr++]);
    return { ERROR, err };
}

// === Tokenize 主流程 ===
vector<Token> tokenize(const string& input) {
    strncpy(buffer, input.c_str(), BUFFER_SIZE);
    bufferEnd = input.size();
    vector<Token> tokens;

    while (forwardptr < bufferEnd) {
        skipWhitespaceAndComments();
        if (forwardptr >= bufferEnd) break;

        lexemeBegin = forwardptr;

        if (isalpha(buffer[forwardptr]) || buffer[forwardptr] == '_')
            tokens.push_back(handleIdentifierOrKeyword());
        else if (isdigit(buffer[forwardptr]))
            tokens.push_back(handleNumber());
        else if (isOperator(buffer[forwardptr]))
            tokens.push_back(handleOperator());
        else if (isDelimiter(buffer[forwardptr]))
            tokens.push_back(handleDelimiter());
        else if (buffer[forwardptr] == '\'')
            tokens.push_back(handleCharConst());
        else if (buffer[forwardptr] == '"')
            tokens.push_back(handleStringConst());
        else
            tokens.push_back(handleError());
    }

    return tokens;
}


// === 打印模块 ===
void printTokens(const vector<Token>& tokens) {
    cout << "Token List:\n";
    for (const auto& token : tokens) {
        cout << "(" << token.lexeme<<","<< tokenTypeToStr(token.type) << ")\n";
    }
}

void printSymbolTable() {
    cout << "\nSymbol Table:\n";
    for (const auto& sym : symbolTable) {
        cout << sym << endl;
    }
}

// === 主程序入口 ===
int main() {
    ifstream fin("D:\\a.c");
    if (!fin.is_open()) {
        cerr << "Unable to open file." << endl;
        return 1;
    }

    string input((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    fin.close();

    if (input.size() >= BUFFER_SIZE) {
        cerr << "Warning: Input truncated due to buffer size limit." << endl;
    }

    auto tokens = tokenize(input);
    printTokens(tokens);
    printSymbolTable();

    return 0;
}
