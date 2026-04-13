#ifndef MYMAKE_PARSER_PARSER_H
#define MYMAKE_PARSER_PARSER_H

#include "parser/ast.h"
#include "parser/lexer.h"
#include <vector>

namespace mymake {

class VariableTable; // forward declare for Phase 2 conditionals

class Parser {
public:
    // Parse logical lines into AST nodes.
    // In Phase 1, only rules (targets + recipes) are parsed.
    ast::Makefile parse(const std::vector<LogicalLine>& lines);

private:
    // Parse a rule line: "target1 target2 : prereq1 prereq2"
    // Collects subsequent recipe lines.
    ast::Rule parse_rule(const std::string& line,
                         const SourceLocation& loc,
                         const std::vector<LogicalLine>& lines,
                         size_t& index);

    // Parse a variable assignment: "VAR = value"
    ast::VariableAssignment parse_assignment(const std::string& line,
                                              const SourceLocation& loc);

    // Split a whitespace-separated string into words.
    std::vector<std::string> split_words(const std::string& text);

    // Find the assignment operator in a line. Returns the position of the
    // operator and sets flavor. Returns std::string::npos if not found.
    size_t find_assignment_op(const std::string& line,
                              ast::VariableAssignment::Flavor& flavor);
};

} // namespace mymake

#endif // MYMAKE_PARSER_PARSER_H
