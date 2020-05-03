#ifndef interpreter_hpp
#define interpreter_hpp
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <utility>
#include <vector>
#include "lexer.h"

enum ExprType { expr_operation, expr_name, expr_const, expr_link };

struct FinalToken
{
  ExprType Type;
  std::string content;

  FinalToken() {
    Type = expr_const;
    content = "0";
  }

  FinalToken(ExprType _Type_, std::string content) : content(std::move(content)) {
    Type = _Type_;
  }
};

enum types { type_string, type_bool, type_int, type_double, type_float, type_char, type_void };

struct variable {
  types type;
  int pointer;

  explicit variable(types type = type_int, int pointer = 0)
      : type(type), pointer(pointer) {}
};

struct r_variable {
  types Type;

  std::string content = "";

  r_variable() { content = ""; }
  r_variable(types _Type, std::string _content) {
    Type = _Type;
    content = std::move(_content);
  }
};

struct token {
  ExprType Type;

  std::string content;

  r_variable* link;

  token() {
    Type = expr_const;
    content = "0";
    link = nullptr;
  }
};

extern std::vector<FinalToken> outputPolish;

extern std::map<std::string,
                std::pair<types, std::vector<std::pair<types, std::string> > > >
    functions;

extern std::map<std::string, int> function_begin;

char opersCharChar(char a, char b, std::string oper);

char opersCharInt(char a, int b, std::string oper);

char opersIntChar(char a, int b, std::string oper);

double opersDoubleDouble(double a, double b, std::string oper);

int opersIntInt(int a, int b, std::string oper);

types toType(std::string& s);

types getType(std::string& s);

r_variable mathOperations(r_variable fir, r_variable sec, std::string oper);

void operation(std::stack<token>& st,
               std::vector<FinalToken> poliz,
               int& ind);

void result(std::vector<FinalToken>& poliz);

#endif /* interpreter_hpp */
