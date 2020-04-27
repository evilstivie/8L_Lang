#include "synsemter.h"

char opersCharChar(char a, char b, std::string oper) {
  if (oper == "+") {
    return a + b;
  } else if (oper == "-") {
    return a - b;
  } else if (oper == "*") {
    return a * b;
  } else if (oper == "/") {
    return a / b;
  } else if (oper == "%") {
    return a % b;
  } else if (oper == "^") {
    return a ^ b;
  } else if (oper == "|") {
    return a | b;
  } else if (oper == "&") {
    return a & b;
  } else if (oper == "||") {
    return a || b;
  } else {
    return a && b;
  }
}

char opersCharInt(char a, int b, std::string oper) {
  if (oper == "+") {
    return a + b;
  } else if (oper == "-") {
    return a - b;
  } else if (oper == "*") {
    return a * b;
  } else if (oper == "/") {
    return a / b;
  } else if (oper == "%") {
    return a % b;
  } else if (oper == "^") {
    return a ^ b;
  } else if (oper == "|") {
    return a | b;
  } else if (oper == "&") {
    return a & b;
  } else if (oper == "||") {
    return a || b;
  } else {
    return a && b;
  }
}

char opersIntChar(char a, int b, std::string oper) {
  if (oper == "+") {
    return a + b;
  } else if (oper == "-") {
    return a - b;
  } else if (oper == "*") {
    return a * b;
  } else if (oper == "/") {
    return a / b;
  } else if (oper == "%") {
    return a % b;
  } else if (oper == "^") {
    return a ^ b;
  } else if (oper == "|") {
    return a | b;
  } else if (oper == "&") {
    return a & b;
  } else if (oper == "||") {
    return a || b;
  } else {
    return a && b;
  }
}

double opersDoubleDouble(double a, double b, std::string oper) {
  if (oper == "+") {
    return a + b;
  } else if (oper == "-") {
    return a - b;
  } else if (oper == "*") {
    return a * b;
  } else if (oper == "/") {
    return a / b;
  } else if (oper == "||") {
    return a || b;
  } else {
    return a && b;
  }
}

char opersIntInt(int a, int b, std::string oper) {
  if (oper == "+") {
    return a + b;
  } else if (oper == "-") {
    return a - b;
  } else if (oper == "*") {
    return a * b;
  } else if (oper == "/") {
    return a / b;
  } else if (oper == "%") {
    return a % b;
  } else if (oper == "^") {
    return a ^ b;
  } else if (oper == "|") {
    return a | b;
  } else if (oper == "&") {
    return a & b;
  } else if (oper == "||") {
    return a || b;
  } else {
    return a && b;
  }
}

types toType(std::string& s) {
  if (s == "int") {
    return type_int;
  } else if (s == "double") {
    return type_double;
  } else if (s == "float") {
    return type_double;
  } else if (s == "bool") {
    return type_bool;
  } else if (s == "string") {
    return type_string;
  } else {
    return type_char;
  }
}

std::vector<std::map<std::string, r_variable>> tid;

types getType(std::string& s) {
  if (s[0] == '\"')
    return type_string;
  if (s[0] == '\'')
    return type_char;
  if (s == "true" || s == "false")
    return type_bool;
  bool foundDot = false;
  for (int i = 0; i < s.size(); i++) {
    if (s[i] == '.') {
      foundDot = true;
      break;
    }
  }
  if (foundDot)
    return type_double;
  return type_int;
}

r_variable mathOperations(r_variable fir, r_variable sec, std::string oper) {
  token will = token();
  std::string ans = "";
  types ansT;

  // bool просто преобразаю в int
  if (fir.content == "true") {
    fir.content = "1";
  }
  if (fir.content == "false") {
    fir.content = "0";
  }
  if (sec.content == "true") {
    sec.content = "1";
  }
  if (sec.content == "false") {
    sec.content = "0";
  }

  if (fir.Type == type_string && oper == "+") {
    //Строки складываем только со строками
    std::string a = fir.content;
    std::string b = sec.content;
    ans = a + b;
    ansT = type_string;
  } else if (fir.Type == type_char || sec.Type == type_char) {
    if (fir.Type == type_char) {
      char a = fir.content[1];
      if (sec.Type == type_char) {
        char b = sec.content[1];
        char c = opersCharChar(a, b, oper);
        ans = "'";
        ans += c;
        ans += "'";
      } else {
        int b = atoi(sec.content.c_str());
        char c = opersCharInt(a, b, oper);
        ans = "'";
        ans += c;
        ans += "'";
      }
    } else {
      int a = atoi(fir.content.c_str());
      char b = sec.content[1];
      char c = opersIntChar(a, b, oper);
      ans = "'";
      ans += c;
      ans += "'";
    }
    ansT = type_char;
  } else {
    if (getType(fir.content) == type_double || getType(sec.content) == type_double) {
      double a = stod(fir.content);
      double b = stod(sec.content);
      double c = opersDoubleDouble(a, b, oper);
      ans = std::to_string(c);

      if (getType(ans) != type_double) {
        ans += ".0";
      }

      ansT = type_double;
    } else {
      int a = atoi(fir.content.c_str());
      int b = atoi(sec.content.c_str());
      int c = opersIntInt(a, b, oper);
      ans = std::to_string(c);
      ansT = type_int;
    }
  }
  r_variable add;
  add.Type = ansT;
  add.content = ans;
  return add;
}

std::map<std::string,
         std::pair<types, std::vector<std::pair<types, std::string>>>>
    functions;

std::map<std::string, int> function_begin;

std::stack<std::pair<int, int>>
    indFunc;  // <количество элементов в tid, индекс в полизе, где был $F>

void toDouble(r_variable& a) {
  if (a.Type == type_int) {
    a.Type = type_double;
    a.content += ".0";
  }
}
void toInt(r_variable& a) {
  if (a.Type == type_char) {
    a.Type = type_int;
    char c = a.content[1];
    int kek = (int)c;
    a.content = std::to_string(kek);
  } else if (a.Type == type_bool) {
    a.Type = type_int;
    if (a.content == "true") {
      a.content = "1";
    } else {
      a.content = "0";
    }
  }
}
r_variable tokenToVar(token a) {
  r_variable b;
  b.content = a.content;
  b.Type = getType(a.content);
  return b;
}

r_variable get_from_tid(std::string name) {
  for (int i = (int)tid.size() - 1; i >= 0; i--) {
    if (tid[i].find(name) != tid[i].end()) {
      return tid[i][name];
    }
  }
  return r_variable();
}

void set_tid(std::string name, r_variable var) {
  for (int i = (int)tid.size() - 1; i >= 0; i--) {
    if (tid[i].find(name) != tid[i].end()) {
      tid[i][name] = var;
      return;
    }
  }
}

void operation(std::stack<token>& st,
               std::vector<FinalToken> poliz,
               int& ind) {
  FinalToken cur = poliz[ind];
  if (cur.content == "+" || cur.content == "-" || cur.content == "*" ||
      cur.content == "/" || cur.content == "%" || cur.content == "^" ||
      cur.content == "|" || cur.content == "&" || cur.content == "||" ||
      cur.content == "&&") {
    token sec = st.top();
    st.pop();
    token fir = st.top();
    st.pop();

    r_variable _fir = tokenToVar(fir);

    r_variable _sec = tokenToVar(sec);

    r_variable ans = mathOperations(_fir, _sec, cur.content);
    token willAdd;
    willAdd.Type = expr_const;
    willAdd.content = ans.content;

    st.push(willAdd);

  } else if (cur.content == "=") {
    token var = st.top();
    st.pop();
    token link_ = st.top();
    st.pop();

    (link_.link)->content = var.content;

    st.push(var);
  } else if (cur.content == "{") {
    tid.emplace_back();
  } else if (cur.content == "}") {
    tid.pop_back();
  } else if (cur.content == "@") {
    token last = st.top();
    st.pop();

    r_variable* per = nullptr;

    for (int i = (int)tid.size() - 1; i >= 0; i--) {
      if (tid[i].find(last.content) != tid[i].end()) {
        per = &tid[i][last.content];
        break;
      }
    }

    token add;
    add.Type = expr_link;
    add.link = per;

    st.push(add);

  } else if (cur.content == "?") {
    token last = st.top();
    st.pop();

    r_variable per = get_from_tid(last.content);

    token add;
    add.Type = expr_const;
    add.content = per.content;

    st.push(add);

  } else if (cur.content == "#") {
    token tokName = st.top();
    st.pop();

    token tokType = st.top();
    st.pop();

    std::pair<types, std::string> p;
    std::string name = tokName.content;
    std::string typeVar = tokType.content;
    types here = toType(typeVar);

    tid[tid.size() - 1][name] = r_variable(here, "0");

    token backType;
    backType.Type = expr_name;
    backType.content = typeVar;

    st.push(backType);

    token varLink;
    varLink.Type = expr_link;
    varLink.content = "";
    for (int i = (int)tid.size() - 1; i >= 0; i--) {
      if (tid[i].find(name) != tid[i].end()) {
        varLink.link = &tid[i][name];
        break;
      }
    }

    st.push(varLink);

  } else if (cur.content == ";") {
    st.pop();
  } else if (cur.content == "#+") {
    token varToken = st.top();

    std::string varName = varToken.content;
    r_variable has = get_from_tid(varName);
    r_variable one = r_variable(type_int, "1");
    r_variable ans = mathOperations(has, one, "+");
    set_tid(varName, ans);

    token ret;
    ret.Type = expr_const;
    ret.content = has.content;
    st.push(ret);

  } else if (cur.content == "#-") {
    token varToken = st.top();

    std::string varName = varToken.content;
    r_variable has = get_from_tid(varName);
    r_variable one = r_variable(type_int, "1");
    r_variable ans = mathOperations(has, one, "-");
    set_tid(varName, ans);

    token ret;
    ret.Type = expr_const;
    ret.content = has.content;
    st.push(ret);

  } else if (cur.content == "+#") {
    token varToken = st.top();

    std::string varName = varToken.content;
    r_variable has = get_from_tid(varName);
    r_variable one = r_variable(type_int, "1");
    r_variable ans = mathOperations(has, one, "+");

    set_tid(varName, ans);

    token ret;
    ret.Type = expr_const;
    ret.content = ans.content;
    st.push(ret);
  } else if (cur.content == "-#") {
    token varToken = st.top();

    std::string varName = varToken.content;
    r_variable has = get_from_tid(varName);
    r_variable one = r_variable(type_int, "1");
    r_variable ans = mathOperations(has, one, "-");
    set_tid(varName, ans);

    token ret;
    ret.Type = expr_const;
    ret.content = ans.content;
    st.push(ret);
  } else if (cur.content == "!A") {
    token nwIndStr = st.top();
    st.pop();
    int nwInd = atoi(nwIndStr.content.c_str());
    ind = nwInd - 1;
  } else if (cur.content == "!F") {
    token nwIndStr = st.top();
    st.pop();
    token needStr = st.top();
    st.pop();
    if (needStr.content == "false") {
      int nwInd = atoi(nwIndStr.content.c_str());
      ind = nwInd - 1;
    }
  } else if (cur.content == "$F") {
    token funcNameTok = st.top();
    st.pop();
    std::string funcName = funcNameTok.content;
    // stack<pair<int, int>> indFunc; //<количество элементов в tid, индекс в
    // полизе, где был $F>
    indFunc.push({tid.size(), ind});
    /*
     map <string, pair <types, vector <pair <types, string> > > > functions;

     map <string, int> function_begin;
     */
    tid.emplace_back();
    int cntParametrs = (int)functions[funcName].second.size();
    while (cntParametrs > 0) {
      token parametr = st.top();
      st.pop();
      r_variable par;
      par.Type = functions[funcName].second[cntParametrs - 1].first;
      par.content = parametr.content;

      std::string parName = functions[funcName].second[cntParametrs - 1].second;

      tid.back()[parName] = par;
      cntParametrs--;
    }

    ind = function_begin[funcName];  // В этом месте точно будет $B, мы его
                                     // просто пропускаем(в for ++)

  } else if (cur.content == "$E") {
    std::pair<int, int> p = indFunc.top();
    indFunc.pop();
    while (tid.size() > p.first) {
      tid.pop_back();
    }
    ind = p.second;
  } else if (cur.content == "==") {
    token firTok = st.top();
    st.pop();
    token secTok = st.top();
    st.pop();

    r_variable fir = tokenToVar(firTok);
    r_variable sec = tokenToVar(secTok);

    token ans;
    ans.Type = expr_const;
    if (fir.Type == type_string) {
      if (fir.content == sec.content) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    } else if (fir.Type == type_double) {
      toDouble(fir);
      toDouble(sec);
      if (std::stof(fir.content) == std::stof(sec.content)) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    } else {
      toInt(fir);
      toInt(sec);
      if (std::stoi(fir.content) == std::stoi(sec.content)) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    }
    st.push(ans);
  } else if (cur.content == ">") {
    token secTok = st.top();
    st.pop();
    token firTok = st.top();
    st.pop();

    r_variable fir = tokenToVar(firTok);
    r_variable sec = tokenToVar(secTok);

    token ans;
    ans.Type = expr_const;
    if (fir.Type == type_string) {
      if (fir.content > sec.content) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    } else if (fir.Type == type_double) {
      toDouble(fir);
      toDouble(sec);
      if (std::stof(fir.content) > std::stof(sec.content)) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    } else {
      toInt(fir);
      toInt(sec);
      if (std::stoi(fir.content) > std::stoi(sec.content)) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    }
    st.push(ans);
  } else if (cur.content == ">=") {
    token secTok = st.top();
    st.pop();
    token firTok = st.top();
    st.pop();

    r_variable fir = tokenToVar(firTok);
    r_variable sec = tokenToVar(secTok);

    token ans;
    ans.Type = expr_const;
    if (fir.Type == type_string) {
      if (fir.content >= sec.content) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    } else if (fir.Type == type_double) {
      toDouble(fir);
      toDouble(sec);
      if (std::stof(fir.content) >= std::stof(sec.content)) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    } else {
      toInt(fir);
      toInt(sec);
      if (std::stoi(fir.content) >= std::stoi(sec.content)) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    }
    st.push(ans);
  } else if (cur.content == "<") {
    token secTok = st.top();
    st.pop();
    token firTok = st.top();
    st.pop();

    r_variable fir = tokenToVar(firTok);
    r_variable sec = tokenToVar(secTok);

    token ans;
    ans.Type = expr_const;
    if (fir.Type == type_string) {
      if (fir.content < sec.content) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    } else if (fir.Type == type_double) {
      toDouble(fir);
      toDouble(sec);
      if (std::stof(fir.content) < std::stof(sec.content)) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    } else {
      toInt(fir);
      toInt(sec);
      if (std::stoi(fir.content) < std::stoi(sec.content)) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    }
    st.push(ans);
  } else if (cur.content == "<=") {
    token secTok = st.top();
    st.pop();
    token firTok = st.top();
    st.pop();

    r_variable fir = tokenToVar(firTok);
    r_variable sec = tokenToVar(secTok);

    token ans;
    ans.Type = expr_const;
    if (fir.Type == type_string) {
      if (fir.content < sec.content) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    } else if (fir.Type == type_double) {
      toDouble(fir);
      toDouble(sec);
      if (fir.content < sec.content) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    } else {
      toInt(fir);
      toInt(sec);
      if (fir.content < sec.content) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    }
    st.push(ans);
  } else if (cur.content == "!=") {
    token firTok = st.top();
    st.pop();
    token secTok = st.top();
    st.pop();

    r_variable fir = tokenToVar(firTok);
    r_variable sec = tokenToVar(secTok);

    token ans;
    ans.Type = expr_const;
    if (fir.Type == type_string) {
      if (fir.content != sec.content) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    } else if (fir.Type == type_double) {
      toDouble(fir);
      toDouble(sec);
      if (std::stof(fir.content) != std::stof(sec.content)) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    } else {
      toInt(fir);
      toInt(sec);
      if (std::stoi(fir.content) != std::stoi(sec.content)) {
        ans.content = "true";
      } else {
        ans.content = "false";
      }
    }
    st.push(ans);
  } else if (cur.content == "+=") {
    token varTok = st.top();
    st.pop();
    token link_ = st.top();
    st.pop();
    r_variable var = tokenToVar(varTok);

    r_variable kek;
    kek.Type = (link_.link)->Type;
    kek.content = (link_.link)->content;

    r_variable debug = mathOperations(kek, var, "+");
    (link_.link)->content = mathOperations(kek, var, "+").content;

    token ans;
    ans.Type = expr_const;
    ans.content = (link_.link)->content;
    st.push(ans);
  } else if (cur.content == "-=") {
    token varTok = st.top();
    st.pop();
    token link_ = st.top();
    st.pop();
    r_variable var = tokenToVar(varTok);

    r_variable kek;
    kek.Type = (link_.link)->Type;
    kek.content = (link_.link)->content;

    (link_.link)->content = mathOperations(kek, var, "-").content;

    token ans;
    ans.Type = expr_const;
    ans.content = (link_.link)->content;
    st.push(ans);
  } else if (cur.content == "*=") {
    token varTok = st.top();
    st.pop();
    token link_ = st.top();
    st.pop();
    r_variable var = tokenToVar(varTok);

    r_variable kek;
    kek.Type = (link_.link)->Type;
    kek.content = (link_.link)->content;

    (link_.link)->content = mathOperations(kek, var, "*").content;

    token ans;
    ans.Type = expr_const;
    ans.content = (link_.link)->content;
    st.push(ans);
  } else if (cur.content == "/=") {
    token varTok = st.top();
    st.pop();
    token link_ = st.top();
    st.pop();
    r_variable var = tokenToVar(varTok);

    r_variable kek;
    kek.Type = (link_.link)->Type;
    kek.content = (link_.link)->content;

    (link_.link)->content = mathOperations(kek, var, "/").content;

    token ans;
    ans.Type = expr_const;
    ans.content = (link_.link)->content;
    st.push(ans);
  } else if (cur.content == "%=") {
    token varTok = st.top();
    st.pop();
    token link_ = st.top();
    st.pop();
    r_variable var = tokenToVar(varTok);

    r_variable kek;
    kek.Type = (link_.link)->Type;
    kek.content = (link_.link)->content;

    (link_.link)->content = mathOperations(kek, var, "%").content;

    token ans;
    ans.Type = expr_const;
    ans.content = (link_.link)->content;
    st.push(ans);
  } else if (cur.content == "|=") {
    token varTok = st.top();
    st.pop();
    token link_ = st.top();
    st.pop();
    r_variable var = tokenToVar(varTok);

    r_variable kek;
    kek.Type = (link_.link)->Type;
    kek.content = (link_.link)->content;

    (link_.link)->content = mathOperations(kek, var, "|").content;

    token ans;
    ans.Type = expr_const;
    ans.content = (link_.link)->content;
    st.push(ans);
  } else if (cur.content == "&=") {
    token varTok = st.top();
    st.pop();
    token link_ = st.top();
    st.pop();
    r_variable var = tokenToVar(varTok);

    r_variable kek;
    kek.Type = (link_.link)->Type;
    kek.content = (link_.link)->content;

    (link_.link)->content = mathOperations(kek, var, "&").content;

    token ans;
    ans.Type = expr_const;
    ans.content = (link_.link)->content;
    st.push(ans);
  } else if (cur.content == "^=") {
    token varTok = st.top();
    st.pop();
    token link_ = st.top();
    st.pop();
    r_variable var = tokenToVar(varTok);

    r_variable kek;
    kek.Type = (link_.link)->Type;
    kek.content = (link_.link)->content;

    (link_.link)->content = mathOperations(kek, var, "^").content;

    token ans;
    ans.Type = expr_const;
    ans.content = (link_.link)->content;
    st.push(ans);
  } else if (cur.content == "!") {
    token varTok = st.top();
    st.pop();

    r_variable var = tokenToVar(varTok);

    token ans;
    ans.Type = expr_const;
    if (var.Type == type_bool) {
      if (var.content == "true") {
        ans.content = "false";
      } else {
        ans.content = "true";
      }
    } else if (var.Type == type_double) {
      double kek = std::atof(var.content.c_str());
      kek = !kek;
      ans.content = std::to_string(kek);
    } else if (var.Type == type_int) {
      int kek = std::atoi(var.content.c_str());
      kek = !kek;
      ans.content = std::to_string(kek);
    } else {
      int kek = var.content[1];
      kek = !kek;
      char c = kek;
      ans.content = "' '";
      ans.content[1] = c;
    }
    st.push(ans);
  } else if (cur.content == "^^") {
    token sec = st.top();
    st.pop();
    token fir = st.top();
    st.pop();

    r_variable lol1 = tokenToVar(fir);
    r_variable lol2 = tokenToVar(sec);

    token ans;
    ans.Type = expr_const;

    if (lol1.Type == type_double || lol2.Type == type_double) {
      double a = std::atof(fir.content.c_str());
      double b = std::atof(sec.content.c_str());

      double c = std::pow(a, b);

      ans.content = std::to_string(c);
    } else {
      int a = std::atoi(fir.content.c_str());
      int b = std::atoi(sec.content.c_str());
      int c = pow(a, b);

      ans.content = std::to_string(c);
    }

    st.push(ans);
  }
}

void result(std::vector<FinalToken>& poliz) {
  std::cout << "Analyzis succeed" << std::endl;
  
  std::stack<token> st;

  indFunc.push({0, poliz.size()});

  for (int i = function_begin["main"]; i < poliz.size(); i++) {
    FinalToken cur = poliz[i];
    if (cur.Type == expr_operation) {
      operation(st, poliz, i);
    } else {
      token ret;
      ret.Type = cur.Type;
      ret.content = cur.content;
      st.push(ret);
    }
    //        std::cout << st.size() << " " << (st.size() > 0 ? st.top().content
    //        : "") << " " << std::endl;
  }

  std::cout << st.top().content << std::endl;
  std::cout << "Program executed successfully" << std::endl;
}
