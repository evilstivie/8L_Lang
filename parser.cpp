#include "parser.h"
#include "synsemter.h"
#include <stack>
#include <string>

//
//  Generation part
//

bool cancel_pushing = false;

std::vector<FinalToken> outputPolish;

void push_operation(const Token &operation) {
  if (!cancel_pushing)
    outputPolish.emplace_back(expr_operation, operation.token);
}

void push_operation(const std::string &operation) {
  if (!cancel_pushing)
    outputPolish.emplace_back(expr_operation, operation);
}

void push_constant(const Token &constant) {
  if (!cancel_pushing)
    outputPolish.emplace_back(expr_const, constant.token);
}

void push_constant(const std::string &constant) {
  if (!cancel_pushing)
    outputPolish.emplace_back(expr_const, constant);
}

void push_name(const Token &name) {
  if (!cancel_pushing)
    outputPolish.emplace_back(expr_name, name.token);
}

void push_name(const std::string &name) {
  if (!cancel_pushing)
    outputPolish.emplace_back(expr_name, name);
}

//
//  End of Generation part
//

std::list<std::map<std::string, variable>> tids;

// Functions
bool open_is_func = false;
types now_function_type;

types give_type(const std::string &_type_) {
  if (_type_ == "string") {
    return type_string;
  } else if (_type_ == "char") {
    return type_char;
  } else if (_type_ == "int") {
    return type_int;
  } else if (_type_ == "double") {
    return type_double;
  } else if (_type_ == "float") {
    return type_float;
  } else if (_type_ == "bool") {
    return type_bool;
  } else if (_type_ == "void") {
    return type_void;
  } else {
    error_in_parser("unknown type");
  }
  return type_int;
}

std::string type_to_string(types _type_) {
  if (_type_ == type_string) {
    return "string";
  } else if (_type_ == type_char) {
    return "char";
  } else if (_type_ == type_int) {
    return "int";
  } else if (_type_ == type_double) {
    return "double";
  } else if (_type_ == type_float) {
    return "float";
  } else if (_type_ == type_bool) {
    return "bool";
  } else if (_type_ == type_void) {
    return "void";
  }
  return "int";
}

variable *find_var(const std::string &var) {
  for (auto &tid : tids) {
    auto st = tid.find(var);
    if (st != tid.end()) {
      return &(st->second);
    }
  }
  error_in_parser("undefined variable");
  return nullptr;
}

void add_var(const std::string &var_type, int ptr, std::string var_name) {
  std::cerr << "New variable " << var_name << std::endl;
  if ((*tids.begin()).find(var_name) != (*tids.begin()).end()) {
    error_in_parser("redifintion of \"" + var_name + "\"");
  }

  int i = 0;
  for (auto tid : tids) {
    for (auto v : tid) {
      if (v.first == var_name) {
        error_in_parser("redifintion of \"" + var_name + "\" in lesser scope");
      }
    }
    i++;
  }

  if (var_type == "string") {
    (*tids.begin())[var_name] = variable(type_string, ptr);
  } else if (var_type == "char") {
    (*tids.begin())[var_name] = variable(type_char, ptr);
  } else if (var_type == "int") {
    (*tids.begin())[var_name] = variable(type_int, ptr);
  } else if (var_type == "double") {
    (*tids.begin())[var_name] = variable(type_double, ptr);
  } else if (var_type == "float") {
    (*tids.begin())[var_name] = variable(type_float, ptr);
  } else if (var_type == "bool") {
    (*tids.begin())[var_name] = variable(type_bool, ptr);
  } else {
    error_in_parser("unknown type of variable");
  }

  push_operation("#");
}

void add_tid(bool is_func = false) {
  if (!is_func) {
    push_operation("{");
  }

  tids.emplace_front();
}

void erase_tid() {
  push_operation("}");
  tids.pop_front();
}

void init_print() {
  functions["print"].first = type_string;
  functions["print"].second = {{type_string, "name"}};
}

void add_function(const std::string &func_type, const std::string &func_name) {
  init_print();
  if (functions.find(func_name) != functions.end()) {
    error_in_parser("redifintion of \"" + func_name + "\"");
  }
  if (func_type == "string") {
    functions[func_name].first = type_string;
  } else if (func_type == "char") {
    functions[func_name].first = type_char;
  } else if (func_type == "int") {
    functions[func_name].first = type_int;
  } else if (func_type == "double") {
    functions[func_name].first = type_double;
  } else if (func_type == "float") {
    functions[func_name].first = type_float;
  } else if (func_type == "bool") {
    functions[func_name].first = type_bool;
  } else if (func_type == "void") {
    functions[func_name].first = type_void;
  } else {
    error_in_parser("unknown type of variable");
  }

  push_operation("##");
}

void check_func(const std::string &func_name) {
  if (functions.find(func_name) != functions.end()) {
    error_in_parser("redifintion of \"" + func_name + "\"");
  }
}

types check_operations(types A, types B, const std::string &oper) {
  if (A == type_string) {
    if (B == type_string) {
      if (oper == "+" || oper == "+=" || oper == "=") {
        return type_string;
      } else if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
                 oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        error_in_parser("invalid operands");
      }
    } else if (B == type_bool) {
      error_in_parser("invalid operands");
    } else if (B == type_int) {
      error_in_parser("invalid operands");
    } else if (B == type_double) {
      error_in_parser("invalid operands");
    } else if (B == type_float) {
      error_in_parser("invalid operands");
    } else if (B == type_char) {
      error_in_parser("invalid operands");
    }
  } else if (A == type_bool) {
    if (B == type_string) {
      error_in_parser("invalid operands");
    } else if (B == type_bool) {
      return type_bool;
    } else if (B == type_int) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_int;
      }
    } else if (B == type_double) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_float) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_float;
      }
    } else if (B == type_char) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_char;
      }
    }
  } else if (A == type_int) {
    if (B == type_string) {
      error_in_parser("invalid operands");
    } else if (B == type_bool) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_int;
      }
    } else if (B == type_int) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_double) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_float) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_char) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    }
  } else if (A == type_double) {
    if (B == type_string) {
      error_in_parser("invalid operands");
    } else if (B == type_bool) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_int) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_double) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_float) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_char) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    }
  } else if (A == type_float) {
    if (B == type_string) {
      error_in_parser("invalid operands");
    } else if (B == type_bool) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_int) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_double) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_float) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_char) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    }
  } else if (A == type_char) {
    if (B == type_string) {
      error_in_parser("invalid operands");
    } else if (B == type_bool) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_int) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_double) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_float) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_double;
      }
    } else if (B == type_char) {
      if (oper == "==" || oper == "!=" || oper == "<" || oper == "<=" ||
          oper == ">" || oper == ">=") {
        return type_bool;
      } else {
        return type_char;
      }
    }
  }
  if (A == type_void || B == type_void) {
    error_in_parser("cannot convert void function to something");
  }
  return type_int;
}

/***********************/
/* syntaxical analyzer */
/***********************/

void error_in_parser(const std::string &_error_) {
  throw std::runtime_error("Error in line: " + std::to_string(line_now) + "\n" +
                           _error_ + "\n");
}

void Parser::target() {
  function(true);
  while (cur_token.type != -1) {
    function(false);
  }
}

void Parser::function(bool first) {
  cancel_pushing = true;

  if (cur_token.token != "func") {
    if (first)
      error_in_parser("Program must start with a function! Found " +
                      cur_token.token);
    else
      error_in_parser("Function was expected, but got " + cur_token.token);
  }

  cur_token = give_token(*fin);
  add_tid();

  std::string type_func;
  std::string func_name = name();

  if (cur_token.type != 3 || cur_token.token != "(") {
    error_in_parser("expected '('");
  }

  cur_token = give_token(*fin);

  std::vector<std::pair<types, std::string>> arguments;

  if (cur_token.type != 3 || cur_token.token != ")") {
    arguments.push_back(description_func());

    while (cur_token.type == 5 && cur_token.token == ",") {
      cur_token = give_token(*fin);

      arguments.push_back(description_func());
    }
  }
  if (cur_token.type != 3 || cur_token.token != ")") {
    error_in_parser("expected ')'");
  }

  cur_token = give_token(*fin);

  if (cur_token.token == ":") {
    cur_token = give_token(*fin);
    type_func = type_function();
  } else {
    type_func = "void";
  }

  add_function(type_func, func_name);

  now_function_type = give_type(type_func);

  cancel_pushing = false;

  function_begin[func_name] = (int)outputPolish.size();

  push_operation("$B");

  add_tid();

  functions[func_name].second = arguments;

  // cur_token = give_token(*fin);

  comp_operator(true);

  erase_tid();

  push_operation("$E");
}

std::string Parser::type_function() {
  if (cur_token.type != 6 ||
      !(cur_token.token == "bool" || cur_token.token == "int" ||
        cur_token.token == "float" || cur_token.token == "char" ||
        cur_token.token == "void" || cur_token.token == "string" ||
        cur_token.token == "double")) {
    error_in_parser("expected function type");
  }
  std::string ans = cur_token.token;

  cur_token = give_token(*fin);

  push_name(ans);

  return ans;
}

std::string Parser::type() {
  if (cur_token.type != 6 ||
      !(cur_token.token == "bool" || cur_token.token == "int" ||
        cur_token.token == "char" || cur_token.token == "float" ||
        cur_token.token == "string" || cur_token.token == "double")) {
    error_in_parser("expected type");
  }

  std::string ans = cur_token.token;

  cur_token = give_token(*fin);

  push_name(ans);

  return ans;
}

std::string Parser::name() {
  if (cur_token.type != 1) {
    error_in_parser("expected name");
  }
  std::string ans = cur_token.token;

  cur_token = give_token(*fin);

  push_name(ans);

  return ans;
}

std::pair<types, std::string> Parser::description_func() {
  cancel_pushing = true;

  std::string type_var;

  type_var = type();

  std::string name_var;

  name_var = name();

  add_var(type_var, 0, name_var);

  cancel_pushing = false;

  return {give_type(type_var), name_var};
}

void Parser::description() {
  std::string _type_;

  _type_ = type();

  section(_type_);

  while (cur_token.type == 5 && cur_token.token == ",") {
    cur_token = give_token(*fin);

    section(_type_);
  }

  if (cur_token.type == 5 && cur_token.token == ";") {
    cur_token = give_token(*fin);

    push_operation(";");

    return;
  } else {
    error_in_parser("expected ';'");
  }
}

void Parser::section(const std::string &_type_) {
  int _pointer_ = 0;

  while (cur_token.type == 4 && cur_token.token == "*") {
    _pointer_++;
    cur_token = give_token(*fin);
  }

  add_var(_type_, _pointer_, name());

  if (cur_token.type != 4 || cur_token.token != "=") {
    push_operation(";");
    return;
  }

  cur_token = give_token(*fin);

  types term_type = term();
  types var_type = give_type(_type_);

  check_operations(var_type, term_type, "=");

  push_operation("=");

  push_operation(";");
}

void Parser::comp_operator(bool is_func = false) {
  if (cur_token.type != 3 || cur_token.token != "{")
    error_in_parser("expected '{'");

  if (!is_func)
    add_tid();

  cur_token = give_token(*fin);
  operator_();

  while (cur_token.type != 3 || cur_token.token != "}") {
    operator_();
  }
  if (cur_token.type != 3 || cur_token.token != "}")
    error_in_parser("expexted '}'");

  if (!is_func)
    erase_tid();

  cur_token = give_token(*fin);
}

void Parser::operator_() {
  if (cur_token.type == 3 && cur_token.token == "{") {
    comp_operator();
  } else if ((cur_token.type == 6 && cur_token.token == "for") ||
             (cur_token.type == 6 && cur_token.token == "while") ||
             (cur_token.type == 6 && cur_token.token == "if") ||
             (cur_token.type == 6 && cur_token.token == "switch")) {
    spec_operator();
  } else if ((cur_token.type == 6 && cur_token.token == "bool") ||
             (cur_token.type == 6 && cur_token.token == "int") ||
             (cur_token.type == 6 && cur_token.token == "char") ||
             (cur_token.type == 6 && cur_token.token == "string") ||
             (cur_token.type == 6 && cur_token.token == "double")) {
    description();
  } else if (cur_token.type == 6 &&
             (cur_token.token == "break" || cur_token.token == "continue")) {
    cur_token = give_token(*fin);

    if (cur_token.type == 2) {
      std::cout << "got number after break " << cur_token.token << std::endl;
      cur_token = give_token(*fin);
    }

    if (cur_token.type != 5 || cur_token.token != ";") {
      error_in_parser("expected ';'");
    }

    cur_token = give_token(*fin);
  } else if (cur_token.type == 6 && cur_token.token == "return") {
    cur_token = give_token(*fin);

    if (cur_token.type == 5 && cur_token.token == ";") {
      if (now_function_type != type_void) {
        error_in_parser("return in non-void function");
      }

      push_operation("$E");

      push_operation(";");

      cur_token = give_token(*fin);
      return;
    }

    types term_type = term();

    check_operations(now_function_type, term_type, "=");

    if (cur_token.type != 5 || cur_token.token != ";") {
      error_in_parser("expected ';'");
    }

    push_operation("$E");

    push_operation(";");

    cur_token = give_token(*fin);
  } else {
    term();

    if (cur_token.type != 5 || cur_token.token != ";") {
      error_in_parser("expected ';'");
    }

    push_operation(";");

    cur_token = give_token(*fin);
  }
}

void Parser::spec_operator() {
  if (cur_token.type == 6 && cur_token.token == "for") {
    operator_for();
  } else if (cur_token.type == 6 && cur_token.token == "if") {
    operator_if();
  } else if (cur_token.type == 6 && cur_token.token == "while") {
    operator_while();
  } else if (cur_token.type == 6 && cur_token.token == "switch") {
    operator_switch();
  } else {
    error_in_parser("expected special operator");
  }
}

void Parser::operator_if() {
  if (cur_token.type != 6 || cur_token.token != "if") {
    error_in_parser("expected \"if\"");
  }

  cur_token = give_token(*fin);

  if (cur_token.type != 3 || cur_token.token != "(") {
    error_in_parser("expected '('");
  }

  cur_token = give_token(*fin);

  term();

  if (cur_token.type != 3 || cur_token.token != ")") {
    error_in_parser("expected ')'");
  }

  size_t pos1 = outputPolish.size();

  push_constant("");

  push_operation("!F");

  cur_token = give_token(*fin);

  operator_();

  if (cur_token.type == 6 && cur_token.token == "else") {
    size_t pos2 = outputPolish.size();

    push_constant("");

    push_operation("!A");

    outputPolish[pos1].content = std::to_string(outputPolish.size());

    operator_else();

    outputPolish[pos2].content = std::to_string(outputPolish.size());
  } else {
    outputPolish[pos1].content = std::to_string(outputPolish.size());
  }
}

void Parser::operator_for() {
  if (cur_token.type != 6 || cur_token.token != "for") {
    error_in_parser("expected \"for\"");
  }

  cur_token = give_token(*fin);

  if (cur_token.type != 3 || cur_token.token != "(") {
    error_in_parser("expected '('");
  }

  add_tid();

  cur_token = give_token(*fin);

  if (cur_token.type != 5 && cur_token.token != ";") {
    if (cur_token.type == 6 &&
        (cur_token.token == "bool" || cur_token.token == "char" ||
         cur_token.token == "float" || cur_token.token == "int")) {
      description();
    } else {
      term();

      if (cur_token.type != 5 && cur_token.token != ";") {
        error_in_parser("expected ';'");
      }

      push_operation(";");

      cur_token = give_token(*fin);
    }
  } else {
    cur_token = give_token(*fin);
  }

  size_t pos3 = outputPolish.size();

  term();

  size_t pos1 = outputPolish.size();

  push_constant("");

  push_operation("!F");

  size_t pos2 = outputPolish.size();

  push_constant("");

  push_operation("!A");

  if (cur_token.type != 5 && cur_token.token != ";") {
    error_in_parser("expected ';'");
  }

  cur_token = give_token(*fin);

  int pos4 = (int)outputPolish.size();

  if (cur_token.type != 3 && cur_token.token != ")") {
    term();

    push_operation(";");
  }

  push_constant(std::to_string(pos3));

  push_operation("!A");

  if (cur_token.type != 3 && cur_token.token != ")") {
    error_in_parser("expexted ')'");
  }

  outputPolish[pos2].content = std::to_string(outputPolish.size());

  cur_token = give_token(*fin);

  operator_();

  push_constant(std::to_string(pos4));

  push_operation("!A");

  outputPolish[pos1].content = std::to_string(outputPolish.size());

  erase_tid();
}

void Parser::operator_while() {
  if (cur_token.type != 6 || cur_token.token != "while") {
    error_in_parser("expected \"while\"");
  }

  cur_token = give_token(*fin);

  if (cur_token.type != 3 || cur_token.token != "(") {
    error_in_parser("expected '('");
  }

  cur_token = give_token(*fin);

  int pos2 = (int)outputPolish.size();

  term();

  size_t pos1 = outputPolish.size();

  push_constant("");

  push_operation("!F");

  if (cur_token.type != 3 || cur_token.token != ")") {
    error_in_parser("expected ')'");
  }

  cur_token = give_token(*fin);

  operator_();

  push_constant(std::to_string(pos2));

  push_operation("!A");

  outputPolish[pos1].content = std::to_string(outputPolish.size());
}

void Parser::operator_switch() {
  if (cur_token.type != 6 || cur_token.token != "switch") {
    error_in_parser("expected \"switch\"");
  }

  cur_token = give_token(*fin);

  add_tid();

  if (cur_token.type != 3 || cur_token.token != "(") {
    error_in_parser("expected '('");
  }

  cur_token = give_token(*fin);

  unsigned int type_position = (int)outputPolish.size();

  push_constant("");

  push_constant("<switch_param>");

  push_operation("#");

  types term_type = term();

  outputPolish[type_position].content = type_to_string(term_type);

  push_operation("=");

  push_operation(";");

  push_operation(";");

  unsigned int y_pos = (int)outputPolish.size();

  push_constant("");

  push_operation("!A");

  if (cur_token.type != 3 || cur_token.token != ")") {
    error_in_parser("expected ')'");
  }

  cur_token = give_token(*fin);

  if (cur_token.type == 3 && cur_token.token == "{") {
    cur_token = give_token(*fin);

    int pos = 0;

    while (cur_token.type != 3 || cur_token.token != "}") {
      y_pos = operator_case(pos, y_pos);

      pos++;
    }

    outputPolish[y_pos].content = std::to_string(outputPolish.size());

    cur_token = give_token(*fin);

    erase_tid();
  }
}

void Parser::operator_else() {
  if (cur_token.type != 6 || cur_token.token != "else") {
    error_in_parser("expected \"else\"");
  }

  cur_token = give_token(*fin);

  operator_();
}

unsigned int Parser::operator_case(int pos, unsigned int prev_case) {
  if (cur_token.type == 6 && cur_token.token == "case") {
    cur_token = give_token(*fin);

    unsigned int y_pos = (int)outputPolish.size();

    push_constant("");

    push_operation("!A");

    outputPolish[prev_case].content = std::to_string(outputPolish.size());

    push_constant("<switch_param>");

    push_operation("?");

    if (cur_token.type != 2) {
      error_in_parser("expected constant");
    }

    push_constant(cur_token);

    push_operation("==");

    cur_token = give_token(*fin);

    unsigned int return_pos = (int)outputPolish.size();

    push_constant("");

    push_operation("!F");

    if (cur_token.type != 5 || cur_token.token != ":") {
      error_in_parser("expected ':'");
    }

    cur_token = give_token(*fin);

    outputPolish[y_pos].content = std::to_string(outputPolish.size());

    while (cur_token.token != "case" && cur_token.token != "default" &&
           cur_token.token != "}") {
      operator_();
    }

    return return_pos;
  } else if (cur_token.type == 6 && cur_token.token == "default" &&
             cur_token.token != "}") {
    cur_token = give_token(*fin);

    outputPolish[prev_case].content = std::to_string(outputPolish.size());

    if (cur_token.type != 5 || cur_token.token != ":") {
      error_in_parser("expected ':'");
    }

    cur_token = give_token(*fin);

    while (cur_token.token != "case" && cur_token.token != "default" &&
           cur_token.token != "}") {
      operator_();
    }
  } else {
    error_in_parser("expected \"case\" or \"default\"");
  }

  return 0;
}

types Parser::term() {
  types type_expr = term_1();

  return type_expr;
}

types Parser::term_1() {
  types type_expr = term_2();

  std::string oper;
  types type_expr2;

  std::stack<std::string> stack_of_oper;

  while (cur_token.type == 4 &&
         (cur_token.token == "=" || cur_token.token == "+=" ||
          cur_token.token == "-=" || cur_token.token == "*=" ||
          cur_token.token == "/=" || cur_token.token == "%=" ||
          cur_token.token == "|=" || cur_token.token == "&=" ||
          cur_token.token == "^=")) {
    outputPolish.pop_back();

    push_operation("@");

    oper = cur_token.token;

    cur_token = give_token(*fin);

    type_expr2 = term_2();

    type_expr = check_operations(type_expr, type_expr2, oper);

    stack_of_oper.push(oper);
  }

  while (!stack_of_oper.empty()) {
    push_operation(stack_of_oper.top());
    stack_of_oper.pop();
  }

  return type_expr;
}

types Parser::term_2() {
  types type_expr = term_3();

  std::string oper;
  types type_expr2;

  while (cur_token.type == 4 && cur_token.token == "||") {
    oper = cur_token.token;

    cur_token = give_token(*fin);

    type_expr2 = term_3();

    type_expr = check_operations(type_expr, type_expr2, oper);

    push_operation(oper);
  }

  return type_expr;
}

types Parser::term_3() {
  types type_expr = term_4();

  std::string oper;
  types type_expr2;

  while (cur_token.type == 4 && cur_token.token == "&&") {
    oper = cur_token.token;

    cur_token = give_token(*fin);

    type_expr2 = term_4();

    type_expr = check_operations(type_expr, type_expr2, oper);

    push_operation(oper);
  }

  return type_expr;
}

types Parser::term_4() {
  types type_expr = term_5();

  std::string oper;
  types type_expr2;

  while (cur_token.type == 4 && cur_token.token == "|") {
    oper = cur_token.token;

    cur_token = give_token(*fin);

    type_expr2 = term_5();

    type_expr = check_operations(type_expr, type_expr2, oper);

    push_operation(oper);
  }

  return type_expr;
}

types Parser::term_5() {
  types type_expr = term_6();

  std::string oper;
  types type_expr2;

  while (cur_token.type == 4 && cur_token.token == "&") {
    oper = cur_token.token;

    cur_token = give_token(*fin);

    type_expr2 = term_6();

    type_expr = check_operations(type_expr, type_expr2, oper);

    push_operation(oper);
  }

  return type_expr;
}

types Parser::term_6() {
  types type_expr = term_7();

  std::string oper;
  types type_expr2;

  while (cur_token.type == 4 &&
         (cur_token.token == "==" || cur_token.token == "!=")) {
    oper = cur_token.token;

    cur_token = give_token(*fin);

    type_expr2 = term_7();

    type_expr = check_operations(type_expr, type_expr2, oper);

    push_operation(oper);
  }

  return type_expr;
}

types Parser::term_7() {
  types type_expr = term_8();

  std::string oper;
  types type_expr2;

  while (cur_token.type == 4 &&
         (cur_token.token == "<" || cur_token.token == "<=" ||
          cur_token.token == ">" || cur_token.token == ">=")) {
    oper = cur_token.token;

    cur_token = give_token(*fin);

    type_expr2 = term_8();

    type_expr = check_operations(type_expr, type_expr2, oper);

    push_operation(oper);
  }

  return type_expr;
}

types Parser::term_8() {
  types type_expr = term_9();

  std::string oper;
  types type_expr2;

  while (cur_token.type == 4 &&
         (cur_token.token == "+" || cur_token.token == "-")) {
    oper = cur_token.token;

    cur_token = give_token(*fin);

    type_expr2 = term_9();

    type_expr = check_operations(type_expr, type_expr2, oper);

    push_operation(oper);
  }

  return type_expr;
}

types Parser::term_9() {
  types type_expr = term_10();

  std::string oper;
  types type_expr2;

  while (cur_token.type == 4 &&
         (cur_token.token == "*" || cur_token.token == "/" ||
          cur_token.token == "%")) {
    oper = cur_token.token;

    cur_token = give_token(*fin);

    type_expr2 = term_10();

    type_expr = check_operations(type_expr, type_expr2, oper);

    push_operation(oper);
  }

  return type_expr;
}

types Parser::term_10() {
  types type_expr = term_11();

  std::string oper;
  types type_expr2;

  while (cur_token.type == 4 && cur_token.token == "^^") {
    oper = cur_token.token;

    cur_token = give_token(*fin);

    type_expr2 = term_11();

    type_expr = check_operations(type_expr, type_expr2, oper);

    push_operation(oper);
  }

  return type_expr;
}

types Parser::term_11() {
  bool is_pref = false;
  std::string oper;

  if (cur_token.type == 4 &&
      (cur_token.token == "++" || cur_token.token == "--" ||
       cur_token.token == "+" || cur_token.token == "-" ||
       cur_token.token == "!" || cur_token.token == "~" ||
       cur_token.token == "*" || cur_token.token == "&" ||
       cur_token.token == "^")) {
    is_pref = true;
    if (cur_token.token == "-") {
      oper = "~-";
    } else if (cur_token.token == "+") {
      oper = "~+";
    } else if (cur_token.token == "++") { // prefix
      oper = "+#";
    } else if (cur_token.token == "--") {
      oper = "-#";
    } else {
      oper = cur_token.token;
    }
    cur_token = give_token(*fin);
    // do smth
  }

  types type_expr = term_12();

  if (is_pref && type_expr == type_string && (oper != "*" && oper != "&")) {
    error_in_parser("cannot perform " + oper + " on a string");
  }

  if (oper.size() > 0) {
    outputPolish.pop_back();

    push_operation(oper);
  }

  return type_expr;
}

types Parser::term_12() {
  types type_expr = term_13();
  std::string oper;

  if (cur_token.type == 4 &&
      (cur_token.token == "++" || cur_token.token == "--")) {
    outputPolish.pop_back();

    if (cur_token.token == "++") { // postfix
      oper = "#+";
    }
    if (cur_token.token == "--") {
      oper = "#-";
    }

    if (type_expr == type_string) {
      error_in_parser("cannot perform " + cur_token.token + " on a string");
    }

    cur_token = give_token(*fin);

    // do smth

    push_operation(oper);
  }

  return type_expr;
}

types Parser::term_13() {
  types type_expr = type_int;

  if (cur_token.type == 3 && cur_token.token == "(") {
    cur_token = give_token(*fin);

    type_expr = term();

    if (cur_token.type != 3 || cur_token.token != ")") {
      error_in_parser("expected ')'");
    }

    cur_token = give_token(*fin);
  } else if (cur_token.type == 1) {
    // function

    Token previous_token = cur_token;

    cur_token = give_token(*fin);

    if (cur_token.type == 3 && cur_token.token == "(") {
      cur_token = give_token(*fin);

      if (functions.find(previous_token.token) == functions.end()) {
        error_in_parser("unknown function");
      }

      unsigned int indx_arguments = 0;

      types argument_type;

      if (cur_token.type != 3 || cur_token.token != ")") {
        argument_type = term();

        if (indx_arguments >= functions[previous_token.token].second.size()) {
          error_in_parser("too much arguments in function");
        }
        if (functions[previous_token.token].second[indx_arguments].first !=
            argument_type) {
          error_in_parser("wrong type of " +
                          std::to_string(indx_arguments + 1) +
                          " argument in function");
        }

        indx_arguments++;

        while (cur_token.type == 5 && cur_token.token == ",") {
          cur_token = give_token(*fin);

          argument_type = term();

          if (indx_arguments >= functions[previous_token.token].second.size()) {
            error_in_parser("too much arguments in function");
          }
          if (functions[previous_token.token].second[indx_arguments].first !=
              argument_type) {
            error_in_parser("wrong type of " +
                            std::to_string(indx_arguments + 1) +
                            " argument in function");
          }

          indx_arguments++;
        }
      }

      if (indx_arguments != functions[previous_token.token].second.size()) {
        error_in_parser("too few arguments in function");
      }

      if (cur_token.type != 3 || cur_token.token != ")") {
        error_in_parser("expected ')'");
      }

      cur_token = give_token(*fin);

      push_name(previous_token);

      push_operation("$F");

      type_expr = functions[previous_token.token].first;
    } else {
      // <name> (previous)

      push_name(previous_token);

      push_operation("?");

      variable *var = find_var(previous_token.token);

      type_expr = (*var).type;
    }
  } else if (cur_token.type == 2) {
    // constant

    push_constant(cur_token);

    if (cur_token.token[0] == '\"') {
      type_expr = type_string;
    } else if (cur_token.token[0] == '\'') {
      type_expr = type_char;
    } else {
      if (cur_token.token.find(".") != std::string::npos) {
        type_expr = type_double;
      } else {
        type_expr = type_int;
      }
    }

    cur_token = give_token(*fin);
  } else if (cur_token.type == 6 &&
             (cur_token.token == "true" || cur_token.token == "false")) {
    // true false

    push_constant(cur_token);

    type_expr = type_bool;

    cur_token = give_token(*fin);
  } else {
    error_in_parser("expected operator");
  }

  return type_expr;
}

void Parser::set(std::ifstream &_input_stream_, std::ofstream &_out_stream_) {
  fin = &_input_stream_;
  fout = &_out_stream_;
}

void Parser::start() {
  try {
    tids.emplace_front();
    cur_token = give_token(*fin);
    target();
    print_poliz();
    result(outputPolish);
    (*fout).close();
  } catch (const std::string &e) {
    std::cout << e << '\n';
  }
}

void Parser::print_poliz() {
  for (int i = 0; i < outputPolish.size(); i++) {
    /**(*fout) << "(";
    if (outputPolish[i].Type == expr_name) {
        (*fout) << "name) ";
    } else if (outputPolish[i].Type == expr_operation) {
        (*fout) << "operation) ";
    } else if (outputPolish[i].Type == expr_const) {
        (*fout) << "constant) ";
    }**/
    (*fout) << outputPolish[i].content << '\n';
  }
}
