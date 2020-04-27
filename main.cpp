#include <cmath>
#include <fstream>
#include <iostream>
#include "lexer.h"
#include "parser.h"

using namespace std;

void parse(ifstream& fin, ofstream& fout) {
  Parser pars;
  pars.set(fin, fout);
  pars.start();

  fin.close();
  fout.close();
}

void print_lexems(ifstream& fin, ofstream& fout) {
  while (!fin.eof()) {
    Token a;
    a = give_token(fin);
    cout << (int)a.type << " " << a.token << endl;
  }
}

int main() {
  ifstream fin;
  ofstream fout;
  fin.open("main.etl");
  fout.open("poliz.txt");
  fin >> noskipws;

  // print_lexems(fin, fout);
  parse(fin, fout);
  return 0;
}
