#include "lexer.h"
#include "synsemter.h"

// states
// 1 - names (strings)
// 2 - numbers (const number or "string" or 'char')
// 3 - brackets ( () [] {} )
// 4 - operaton ( + - * / % ^ = += -= *= /= %= ^= | & || && )
// 5 - punkt ( , ; )
// 6 - reserved words

long long line_now = 1;

std::unordered_set<std::string> reserved_words{
        "func", "bool", "int", "float", "double", "char", "void",
        "return", "true", "false", "break", "continue", "switch", "case",
        "default", "if", "else", "while", "for", "string"};

inline bool is_word(const char &x) {
    return ('a' <= x && x <= 'z') || ('A' <= x && x <= 'Z');
}

inline bool is_number(const char &x) {
    return '0' <= x && x <= '9';
}

inline bool is_bracket(const char &x) {
    return x == '(' || x == ')' || x == '[' || x == ']' || x == '{' || x == '}';
}

void error_in_lexical(const std::string &_error_) {
    throw std::runtime_error("Error in line: " + std::to_string(line_now) + "\n" + _error_ + "\n");
}

enum States {
    A, B, C, D, E, F, G, H, J, K, com1, com2, com3
};

Token give_token(std::ifstream &fin) {
    std::string token;
    token.reserve(10);

    States state;
    state = A;

    char now;

    while (fin >> now) {
        if (now == '\n') {
            line_now++;
        }
        switch (state) {
            case A:
                if (is_word(now) || now == '_') {
                    token += now;
                    state = B;
                } else if (is_number(now)) {
                    token += now;
                    state = C;
                } else if (now == '"') {
                    token += now;
                    state = D;
                } else if (now == '\'') {
                    token += now;
                    state = E;
                } else if (is_bracket(now)) {
                    token += now;
                    return {3, token};
                } else if (now == ',' || now == ';' || now == ':') {
                    token += now;
                    return {5, token};
                } else if (now == '/') {
                    state = G;
                } else if (now == '=' || now == '+' || now == '-' || now == '*' ||
                           now == '%' || now == '|' || now == '&' || now == '^' ||
                           now == '<' || now == '>' || now == '!' || now == '~') {
                    token += now;
                    state = H;
                } else if (now == ' ' || now == '\n') {
                    break;
                } else {
                    error_in_lexical((std::string) "unknown character: '" + now +
                                     (std::string) "'");
                }
                break;

            case B:
                if (is_word(now) || is_number(now) || now == '_') {
                    token += now;
                } else {
                    fin.seekg((int) fin.tellg() - 1);
                    if (reserved_words.find(token) == reserved_words.end())
                        return {1, token};
                    else
                        return {6, token};
                }
                break;

            case C:
                if (is_number(now)) {
                    token += now;
                } else if (now == '.') {
                    token += now;
                    state = J;
                } else if (is_word(now)) {
                    error_in_lexical((std::string) "unexpected character: '" + now +
                                     (std::string) "'");
                } else {
                    fin.seekg((int) fin.tellg() - 1);
                    return {2, token};
                }
                break;

            case D:
                if (now == '"') {
                    token += now;
                    return {2, token};
                } else {
                    token += now;
                }
                break;

            case E:
                if (now == '\\') {
                    fin >> now;
                    if (now != '\'') {
                        error_in_lexical((std::string) "unexpected character: '" + now +
                                         (std::string) "'");
                    }
                }
                token += now;
                state = F;
                break;

            case F:
                if (now == '\'') {
                    token += now;
                    return {2, token};
                } else {
                    error_in_lexical((std::string) "unexpected character: '" + now +
                                     (std::string) "'");
                }
                break;

            case G:
                if (now == '/') {
                    state = com1;
                } else if (now == '*') {
                    state = com2;
                } else if (now == '=') {
                    token += "/=";
                    return {4, token};
                } else {
                    fin.seekg((int) fin.tellg() - 1);
                    token += '/';
                    return {4, token};
                }
                break;

            case H:
                if (now == '=') {
                    if (token == "~") {
                        error_in_lexical((std::string) "unexpected character: '" + now +
                                         (std::string) "'");
                    } else {
                        token += now;
                        return {4, token};
                    }
                } else if (now == '|') {
                    if (token == "|") {
                        token += now;
                        return {4, token};
                    } else {
                        error_in_lexical((std::string) "unexpected character: '" + now +
                                         (std::string) "'");
                    }
                } else if (now == '&') {
                    if (token == "&") {
                        token += now;
                        return {4, token};
                    } else {
                        error_in_lexical((std::string) "unexpected character: '" + now +
                                         (std::string) "'");
                    }
                } else if (now == '+') {
                    if (token == "+") {
                        token += now;
                        return {4, token};
                    } else {
                        error_in_lexical((std::string) "unexpected character: '" + now +
                                         (std::string) "'");
                    }
                } else if (now == '-') {
                    if (token == "-") {
                        token += now;
                        return {4, token};
                    } else {
                        error_in_lexical((std::string) "unexpected character: '" + now +
                                         (std::string) "'");
                    }
                } else if (now == '^') {
                    if (token == "^") {
                        token += now;
                        return {4, token};
                    } else {
                        error_in_lexical((std::string) "unexpected character: '" + now +
                                         (std::string) "'");
                    }
                } else {
                    fin.seekg((int) fin.tellg() - 1);
                    return {4, token};
                }
                break;

            case J:
                if (is_number(now)) {
                    token += now;
                    state = K;
                } else {
                    error_in_lexical((std::string) "unexpected character: '" + now +
                                     (std::string) "'");
                }
                break;

            case K:
                if (is_number(now)) {
                    token += now;
                } else if (is_word(now)) {
                    error_in_lexical((std::string) "unexpected character: '" + now +
                                     (std::string) "'");
                } else {
                    fin.seekg((int) fin.tellg() - 1);
                    return {2, token};
                }
                break;

            case com1:
                if (now == '\n') {
                    state = A;
                }
                break;

            case com2:
                if (now == '*') {
                    state = com3;
                }
                break;

            case com3:
                if (now == '/') {
                    state = A;
                } else if (now != '*') {
                    state = com2;
                }
                break;
        }
    }

    return {-1, "end"};  // Error
}

// must return pair <char, string>
