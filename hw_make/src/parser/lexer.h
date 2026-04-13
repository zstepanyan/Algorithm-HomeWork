#ifndef MYMAKE_PARSER_LEXER_H
#define MYMAKE_PARSER_LEXER_H

#include "core/source_location.h"
#include <string>
#include <vector>

namespace mymake {

struct LogicalLine {
    enum Kind { Recipe, Directive, Assignment, RuleOrOther, Empty };
    Kind kind;
    std::string content;
    SourceLocation location;
};

class Lexer {
public:
    // Lex a file by path. Throws MakeError on I/O failure.
    std::vector<LogicalLine> lex_file(const std::string& filename);

    // Lex from a string (used by $(eval ...) and tests).
    std::vector<LogicalLine> lex_string(const std::string& text,
                                        const std::string& source_name = "<string>");

private:
    std::vector<LogicalLine> lex(const std::string& text,
                                 const std::string& source_name);

    // Join backslash-newline continuations, returning logical lines
    // with their starting physical line numbers.
    struct RawLine {
        std::string text;
        int start_line;
    };
    std::vector<RawLine> join_continuations(const std::string& text);

    // Classify a logical line by its content.
    LogicalLine::Kind classify(const std::string& line, bool in_rule);

    // Strip inline comments from a non-recipe line.
    // Respects variable references: $(#) is not a comment.
    std::string strip_comment(const std::string& line);
};

} // namespace mymake

#endif // MYMAKE_PARSER_LEXER_H
