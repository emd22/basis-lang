#pragma once

#include <string>

namespace basis {
    struct Token {
        enum {
            TK_NONE,
            TK_NUMBER,
            TK_STRING,
            TK_IDENT,
            TK_KEYWORD,
            TK_OPERATOR,
        } type = TK_NONE;

        std::string value;
    };
}