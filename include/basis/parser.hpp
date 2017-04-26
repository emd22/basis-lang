#pragma once

#include <map>
#include <string>
#include <stack>
#include <vector>

#include <basis/token.hpp>
#include <basis/variable.hpp>

namespace basis {

class Parser {
public:
    Parser(const std::vector<Token> &tokens);

    void Parse();
    void Next();

    void ParseVarDecl();
    void PrintStmt();

private:
    // expects a token of a certain type to be at the index.
    // returns true if found. (if found, out_token will be set to the found token)
    // if read is true, increment m_token_index
    void Expect(int token_type, Token &out_token, bool read);
    bool Match(int token_type, const std::string &value, Token &out_token, bool read);

    std::vector<Token> m_tokens;
    int m_token_index;

    struct {
        std::map<std::string, Variable> vars;
        std::stack<Variable> stack;
    } m_state;
};

}