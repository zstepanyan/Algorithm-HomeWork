#include "parser/lexer.h"
#include "core/errors.h"
#include <fstream>
#include <sstream>

namespace mymake {

std::vector<LogicalLine> Lexer::lex_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw MakeError("No rule to make target '" + filename + "'",
                        SourceLocation(filename, 0));
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return lex(ss.str(), filename);
}

std::vector<LogicalLine> Lexer::lex_string(const std::string& text,
                                            const std::string& source_name) {
    return lex(text, source_name);
}

std::vector<LogicalLine> Lexer::lex(const std::string& text,
                                     const std::string& source_name) {
    auto raw_lines = join_continuations(text);
    std::vector<LogicalLine> result;
    bool in_rule = false;

    for (auto& raw : raw_lines) {
        auto kind = classify(raw.text, in_rule);

        if (kind == LogicalLine::Empty) {
            in_rule = false;
            continue;
        }

        std::string content;
        if (kind == LogicalLine::Recipe) {
            // Strip leading tab, keep everything else verbatim
            content = raw.text.substr(1);
        } else {
            content = strip_comment(raw.text);
            // Trim leading/trailing whitespace for non-recipe lines
            auto start = content.find_first_not_of(" \t");
            if (start == std::string::npos) {
                continue; // became empty after comment strip
            }
            auto end = content.find_last_not_of(" \t\r\n");
            content = content.substr(start, end - start + 1);
        }

        if (kind == LogicalLine::RuleOrOther) {
            in_rule = true;
        }

        result.push_back({kind, std::move(content),
                          SourceLocation(source_name, raw.start_line)});
    }

    return result;
}

std::vector<Lexer::RawLine> Lexer::join_continuations(const std::string& text) {
    std::vector<RawLine> lines;
    std::string current;
    int start_line = 1;
    int physical_line = 1;
    bool continuing = false;

    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];

        if (c == '\n') {
            if (continuing) {
                // Replace backslash-newline with a single space
                if (!current.empty() && current.back() == '\\') {
                    current.pop_back();
                }
                current += ' ';
                continuing = false;
            } else {
                lines.push_back({current, start_line});
                current.clear();
                start_line = physical_line + 1;
            }
            physical_line++;
            continue;
        }

        if (c == '\\' && i + 1 < text.size() && text[i + 1] == '\n') {
            current += '\\';
            continuing = true;
            continue;
        }

        current += c;
    }

    // Handle last line without trailing newline
    if (!current.empty() || continuing) {
        if (continuing && !current.empty() && current.back() == '\\') {
            current.pop_back();
        }
        lines.push_back({current, start_line});
    }

    return lines;
}

LogicalLine::Kind Lexer::classify(const std::string& line, bool in_rule) {
    if (line.empty()) {
        return LogicalLine::Empty;
    }

    // Check if it's only whitespace
    bool only_whitespace = true;
    for (char c : line) {
        if (c != ' ' && c != '\t' && c != '\r') {
            only_whitespace = false;
            break;
        }
    }
    if (only_whitespace) {
        return LogicalLine::Empty;
    }

    // Recipe lines start with a tab
    if (line[0] == '\t') {
        if (in_rule) {
            return LogicalLine::Recipe;
        }
        // Tab-indented line outside a rule context — still treat as recipe
        // (GNU make would error "recipe commences before first target")
        return LogicalLine::Recipe;
    }

    // Comment-only lines (after stripping leading whitespace)
    auto first_nonspace = line.find_first_not_of(" \t");
    if (first_nonspace != std::string::npos && line[first_nonspace] == '#') {
        return LogicalLine::Empty;
    }

    // Check for directives
    std::string trimmed = line.substr(first_nonspace);
    if (trimmed.starts_with("include ") || trimmed.starts_with("include\t") ||
        trimmed.starts_with("-include ") || trimmed.starts_with("-include\t") ||
        trimmed.starts_with("sinclude ") || trimmed.starts_with("sinclude\t") ||
        trimmed.starts_with("vpath ") || trimmed.starts_with("vpath\t") ||
        trimmed.starts_with("export ") || trimmed.starts_with("export\t") ||
        trimmed.starts_with("unexport ") || trimmed.starts_with("unexport\t") ||
        trimmed.starts_with("override ") || trimmed.starts_with("override\t") ||
        trimmed.starts_with("ifeq ") || trimmed.starts_with("ifeq(") ||
        trimmed.starts_with("ifneq ") || trimmed.starts_with("ifneq(") ||
        trimmed.starts_with("ifdef ") || trimmed.starts_with("ifdef\t") ||
        trimmed.starts_with("ifndef ") || trimmed.starts_with("ifndef\t") ||
        trimmed == "else" || trimmed.starts_with("else ") ||
        trimmed == "endif" || trimmed.starts_with("endif ") ||
        trimmed.starts_with("endif\t")) {
        return LogicalLine::Directive;
    }

    // Check for assignment: scan for =, :=, +=, ?= before an unparenthesized ':'
    int paren_depth = 0;
    for (size_t i = first_nonspace; i < line.size(); ++i) {
        char c = line[i];
        if (c == '(' || c == '{') {
            paren_depth++;
        } else if (c == ')' || c == '}') {
            paren_depth--;
        } else if (paren_depth == 0) {
            if (c == '=' ||
                (c == ':' && i + 1 < line.size() && line[i + 1] == '=') ||
                (c == '+' && i + 1 < line.size() && line[i + 1] == '=') ||
                (c == '?' && i + 1 < line.size() && line[i + 1] == '=')) {
                // But make sure we didn't just hit '=' inside a rule's prereqs
                // Check if there was a ':' before this '='
                bool colon_seen = false;
                for (size_t j = first_nonspace; j < i; ++j) {
                    if (line[j] == '(' || line[j] == '{') paren_depth++;
                    else if (line[j] == ')' || line[j] == '}') paren_depth--;
                    else if (line[j] == ':' && paren_depth == 0 &&
                             !(j + 1 < line.size() && line[j + 1] == '=')) {
                        colon_seen = true;
                        break;
                    }
                }
                if (!colon_seen) {
                    return LogicalLine::Assignment;
                }
                break; // it's a rule with target-specific variable
            }
            if (c == ':' && !(i + 1 < line.size() && line[i + 1] == '=')) {
                break; // found rule separator first
            }
        }
    }

    return LogicalLine::RuleOrOther;
}

std::string Lexer::strip_comment(const std::string& line) {
    std::string result;
    int paren_depth = 0;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        // Track $( ... ) and ${ ... } to avoid stripping # inside references
        if (c == '$' && i + 1 < line.size()) {
            char next = line[i + 1];
            if (next == '(' || next == '{') {
                result += c;
                result += next;
                paren_depth++;
                i++;
                continue;
            }
        }

        if ((c == ')' || c == '}') && paren_depth > 0) {
            paren_depth--;
            result += c;
            continue;
        }

        // Escaped hash
        if (c == '\\' && i + 1 < line.size() && line[i + 1] == '#') {
            result += '#';
            i++;
            continue;
        }

        // Unescaped # outside variable reference is a comment
        if (c == '#' && paren_depth == 0) {
            break;
        }

        result += c;
    }

    return result;
}

} // namespace mymake
