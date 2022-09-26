//
// Created by suzuco on 3/11/22.
//

#include "exprtk/exprtk.hpp"

std::string eval(const std::string & expr_str)
{
    std::string result;

    exprtk::symbol_table<double> symtbl;
    symtbl.add_constants();

    exprtk::expression<double> expr;
    expr.register_symbol_table(symtbl);

    exprtk::parser<double> parser;
    if (!parser.compile(expr_str, expr)) {
        result += parser.error();
    }
    else {
        result += expr_str + "\n= ";
        result += std::to_string(expr.value());
    }
    return result;
}