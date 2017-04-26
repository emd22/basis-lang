#include <cctype>
#include <iostream>
#include <algorithm>

#include <basis/lexer.hpp>
#include <basis/keywords.hpp>

namespace basis {
Lexer::Lexer(File &file) 
    : m_file(file), m_scan_pos(0) {
}

char Lexer::PeekChar() {
    if (m_scan_pos >= m_file.data.size()) {
        return '\0';
    }
    return m_file.data.at(m_scan_pos);
}
char Lexer::ReadChar() {
    return m_file.data.at(m_scan_pos++);
}
void Lexer::SkipWhitespace() {
    while (std::isspace(PeekChar())) {
        ReadChar();
    }
}

std::vector<Token> Lexer::Lex() {
    std::vector<Token> tokens;
    SkipWhitespace();
    while (char ch = PeekChar()) {
        if (std::isalpha(ch)) {
            tokens.push_back(ReadIdentifier());
        } else if (std::isdigit(ch)) {
            tokens.push_back(ReadNumber());
        } else if (ch == '\'' || ch == '"') {
            tokens.push_back(ReadString());
        } else if (ch == '+' || ch == '-'
                || ch == '*' || ch == '/'
                || ch == '=') {
            m_scan_pos++;
            Token res;
            res.type = Token::TK_OPERATOR;
            res.value += ch;
            tokens.push_back(res);
        } else {
            char bad_char = ReadChar();
            std::cout << "Unknown char: " << bad_char << "\n";
        }

        SkipWhitespace();
    }
    return tokens;
}

Token Lexer::ReadIdentifier() {
    Token res;
    res.type = Token::TK_IDENT;

    char ch;

    do {
        ch = ReadChar();
        res.value += ch;
    } while (std::isalnum(PeekChar()));

    auto it = std::find(Keywords::keywords.begin(), Keywords::keywords.end(), res.value);

    if (it != Keywords::keywords.end()) {
        res.type = Token::TK_KEYWORD;    
    }

    return res;
}

Token Lexer::ReadNumber() {
    Token res;
    res.type = Token::TK_NUMBER;

    bool decimal_hit = false;

    char ch = PeekChar();
    while (ch != '\0' && std::isdigit(ch)) {
        // eat the char
        ReadChar();

        res.value += ch;

        if (PeekChar() == '.') {
            if (decimal_hit) {
                // decimal already hit, drop out.
                ch = '\0';
                break;
            }

            // eat the decimal. yum!
            res.value += '.';
            ReadChar();

            decimal_hit = true;
        }

        // next!
        ch = PeekChar();
    }

    return res;
}

Token Lexer::ReadString() {
    Token res;
    res.type = Token::TK_STRING;

    const char delim = ReadChar(); // eat the "
    char pc = PeekChar();

    while (pc != '\0' && pc != delim) {
        res.value += pc;
        ReadChar();
        pc = PeekChar();
    }

    if (pc == delim) {
        ReadChar();
    }

    return res;
}

}