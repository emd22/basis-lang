#include <iostream>
#include <sstream>

#include <basis/parser.hpp>

namespace basis {

Parser::Parser(const std::vector<Token> &tokens)
    : m_tokens(tokens), m_token_index(0) {

}

bool Parser::Match(int token_type, const std::string &value, Token &out_token, bool read) {
    if (m_token_index >= m_tokens.size()) {
        std::stringstream ss;
        
        ss << "Run-time error! Unexpected EOF.\n";
        std::string ex = ss.str();
        throw ex;
    } else if (m_tokens[m_token_index].type == token_type && m_tokens[m_token_index].value == value) {
        out_token = m_tokens[m_token_index];
        if (read) {
            m_token_index++;
        }

        return true;

    } else {
        return false;
    }
}

void Parser::Expect(int token_type, Token &out_token, bool read) {
    if (m_token_index >= m_tokens.size()) {
        std::stringstream ss;
        
        ss << "Run-time error! Unexpected EOF.\n";
        std::string ex = ss.str();
        throw ex;
    } else if (m_tokens[m_token_index].type == token_type) {
        out_token = m_tokens[m_token_index];
        if (read) {
            m_token_index++;
        }
    } else {
        std::stringstream ss;
        
        ss << "Run-time error! Expected token of type '" << token_type << " instead got: " << m_tokens[m_token_index].type << "\n";

        std::string ex = ss.str();
        throw ex;
    }
}

void Parser::Parse() {
    while (m_token_index < m_tokens.size()) {
        try {
            Next();
        } catch (std::string ex) {
            std::cout << "Run-time error: " << ex << "\n";
            return;
        }
    }
}

void Parser::Next() {
    if (m_token_index >= m_tokens.size()) {
        throw std::string("Unexpected EOF");
    }

    const Token current = m_tokens[m_token_index];

    switch (current.type) {
        case Token::TK_KEYWORD: {
            if (current.value == "var") {
                ParseVarDecl();
            } else if (current.value == "print") {
                PrintStmt();
            } else {
                std::stringstream ss;
                ss << "Unknown keyword '" << current.value << "'";
                std::string ex = ss.str();
                throw ex;
            }

            break;
        }
        case Token::TK_OPERATOR: {
            m_token_index++;
            break;
        }
        case Token::TK_STRING:
            // fallthrough
        case Token::TK_NUMBER: {
            // put the number on the top of the stack
            m_state.stack.push(Variable { current.value });

            m_token_index++;

            break;
        }
        case Token::TK_IDENT: {
            auto it = m_state.vars.find(current.value);
            if (it != m_state.vars.end()) {
                m_state.stack.push(it->second);
                m_token_index++;
            } else {
                std::stringstream ss;
                ss << "Undeclared variable '" << current.value << "'";
                std::string ex = ss.str();
                throw ex;
            }


            break;
        }
        default: {
            std::stringstream ss;
            ss << "Unknown token '" << current.value << "'";
            std::string ex = ss.str();
            throw ex;
        }
    }
}

void Parser::ParseVarDecl() {
    // read 'var'
    m_token_index++;

    Token name_token, opr_token;
    Variable var_value {"null"};
    // expect name
    Expect(Token::TK_IDENT, name_token, true);
    if (Match(Token::TK_OPERATOR, "=", opr_token, true)) {
        Next();
        var_value = m_state.stack.top();
        m_state.stack.pop();
    }

    // check for redeclared
    if (m_state.vars.find(name_token.value) == m_state.vars.end()) {
        m_state.vars[name_token.value] = var_value;
    } else {
        std::stringstream ss;
        ss << "Variable '" << name_token.value << "' redeclared";
        std::string ex = ss.str();
        throw ex;
    }
}

void Parser::PrintStmt() {
    m_token_index++;
    // read next part
    Next();

    if (m_state.stack.size() == 0) {
        throw std::string("Stack empty");
    }

    // print top item on stack
    std::cout << m_state.stack.top().value << "\n";

    // pop top from stack
    m_state.stack.pop();
}

}