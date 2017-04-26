#pragma once

#include <vector>

#include <basis/token.hpp>
#include <basis/file.hpp>

namespace basis {

class Lexer {
public:
    Lexer(File &file);
    char PeekChar();
    char ReadChar();
    void SkipWhitespace();
    std::vector<Token> Lex();
    Token ReadIdentifier();
    Token ReadNumber();
    Token ReadString();
private:
    File m_file;
    int m_scan_pos;
};

}