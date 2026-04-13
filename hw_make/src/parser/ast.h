#ifndef MYMAKE_PARSER_AST_H
#define MYMAKE_PARSER_AST_H

#include "core/source_location.h"
#include <string>
#include <variant>
#include <vector>

namespace mymake::ast {

using Expression = std::string; // raw text, may contain $(VAR) references

struct VariableAssignment {
    enum Flavor { Recursive, Simple, Append, Conditional }; // =, :=, +=, ?=
    std::string name;
    Expression value;
    Flavor flavor;
    bool is_override = false;
    bool is_export = false;
    SourceLocation loc;
};

struct Rule {
    std::vector<Expression> targets;
    std::vector<Expression> normal_prereqs;
    std::vector<Expression> order_only_prereqs;
    std::vector<Expression> recipes;
    bool is_pattern = false;
    bool is_double_colon = false;
    SourceLocation loc;
};

struct Include {
    std::vector<Expression> files;
    bool is_silent = false;
    SourceLocation loc;
};

struct VpathDirective {
    std::string pattern;
    std::vector<std::string> directories;
    SourceLocation loc;
};

struct Export {
    std::string variable_name;
    bool unexport = false;
    SourceLocation loc;
};

using Node = std::variant<VariableAssignment, Rule, Include, VpathDirective, Export>;
using Makefile = std::vector<Node>;

} // namespace mymake::ast

#endif // MYMAKE_PARSER_AST_H
