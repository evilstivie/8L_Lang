#ifndef lexical_analyzer_hpp
#define lexical_analyzer_hpp

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>

extern long long line_now;

struct Token {
    char type{};
    std::string token;

    Token() = default;

    Token(char _type_, std::string _token_) {
        type = _type_;
        token = std::move(_token_);
    }
};

inline bool is_word(const char &x);

inline bool is_number(const char &x);

inline bool is_bracket(const char &x);

void error_in_lexical();

void error_in_lexical(const std::string&);

Token give_token(std::ifstream &fin);

#endif /* lexical_analyzer_hpp */
