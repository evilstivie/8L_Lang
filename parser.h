#ifndef parser_hpp
#define parser_hpp

#include <list>
#include <map>
#include <set>
#include <vector>
#include "synsemter.h"
#include "lexer.h"

void error_in_parser();

void error_in_parser(const std::string &);

extern std::list<std::map<std::string, variable> > tids;

types check_operations(types A, types B, const std::string &op);

class Parser {
	public:
		void init_print();

private:
    std::ifstream *fin{};
    std::ofstream *fout{};
    Token cur_token;

    // Analyzer

    void target();

    void function(bool first = false);

    std::string type_function();

    std::string type();

    std::string name();

    std::pair<types, std::string> description_func();

    void description();

    void section(const std::string& _type_);

    void comp_operator(bool is_func);

    void operator_();

    void spec_operator();

    void operator_if();

    void operator_for();

    void operator_while();

    void operator_switch();

    void operator_else();

    unsigned int operator_case(int pos = 0, unsigned int prev_case = 0);

    types term();

    types term_1();

    types term_2();

    types term_3();

    types term_4();

    types term_5();

    types term_6();

    types term_7();

    types term_8();

    types term_9();

    types term_10();

    types term_11();

    types term_12();

    types term_13();

public:
    void set(std::ifstream &_input_stream_, std::ofstream &_out_stream_);

    void start();

    void print_poliz();
};

#endif /* parser_hpp */
