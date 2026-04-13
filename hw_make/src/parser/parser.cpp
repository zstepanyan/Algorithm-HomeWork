#include "parser/parser.h"
#include "core/errors.h"
#include <algorithm>

namespace mymake {

ast::Makefile Parser::parse(const std::vector<LogicalLine>& lines) {
    ast::Makefile makefile;

    for (size_t i = 0; i < lines.size(); ++i) {
        const auto& line = lines[i];

        switch (line.kind) {
        case LogicalLine::Recipe:
            // Stray recipe line outside a rule
            throw ParseError("recipe commences before first target",
                             line.location);

        case LogicalLine::Assignment: {
            auto assignment = parse_assignment(line.content, line.location);
            makefile.push_back(std::move(assignment));
            break;
        }

        case LogicalLine::RuleOrOther: {
            auto rule = parse_rule(line.content, line.location, lines, i);
            makefile.push_back(std::move(rule));
            break;
        }

        case LogicalLine::Directive:
            // Phase 1: skip directives (include, conditionals, etc.)
            // They will be handled in later phases.
            break;

        case LogicalLine::Empty:
            break;
        }
    }

    return makefile;
}

ast::Rule Parser::parse_rule(const std::string& line,
                              const SourceLocation& loc,
                              const std::vector<LogicalLine>& lines,
                              size_t& index) {
    ast::Rule rule;
    rule.loc = loc;

    // Find the colon separator (skip colons inside $(...) or ${...})
    size_t colon_pos = std::string::npos;
    int paren_depth = 0;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '$' && i + 1 < line.size() &&
            (line[i + 1] == '(' || line[i + 1] == '{')) {
            paren_depth++;
            i++;
        } 
        else if ((c == ')' || c == '}') && paren_depth > 0) {
            paren_depth--;
        } 
        else if (c == ':' && paren_depth == 0) {
            // Check for :: (double-colon rule)
            if (i + 1 < line.size() && line[i + 1] == ':') {
                rule.is_double_colon = true;
                colon_pos = i;
                break;
            }
            // Check it's not := (assignment operator)
            if (i + 1 < line.size() && line[i + 1] == '=') {
                continue;
            }
            colon_pos = i;
            break;
        }
    }

    if (colon_pos == std::string::npos) {
        throw ParseError("missing separator", loc);
    }

    // Parse targets (left of colon)
    std::string targets_str = line.substr(0, colon_pos);
    rule.targets = split_words(targets_str);

    if (rule.targets.empty()) {
        throw ParseError("missing target", loc);
    }

    // Check if any target contains '%' (pattern rule)
    for (const auto& t : rule.targets) {
        if (t.find('%') != std::string::npos) {
            rule.is_pattern = true;
            break;
        }
    }

    // Parse prerequisites (right of colon)
    size_t prereq_start = colon_pos + 1;
    if (rule.is_double_colon) {
        prereq_start = colon_pos + 2;
    }

    std::string prereqs_str;
    std::string inline_recipe;

    // Check for inline recipe after ';'
    size_t semicolon = std::string::npos;
    paren_depth = 0;
    for (size_t i = prereq_start; i < line.size(); ++i) {
        char c = line[i];
        if (c == '$' && i + 1 < line.size() &&
            (line[i + 1] == '(' || line[i + 1] == '{')) {
            paren_depth++;
            i++;
        } 
        else if ((c == ')' || c == '}') && paren_depth > 0) {
            paren_depth--;
        } 
        else if (c == ';' && paren_depth == 0) {
            semicolon = i;
            break;
        }
    }

    if (semicolon != std::string::npos) {
        prereqs_str = line.substr(prereq_start, semicolon - prereq_start);
        inline_recipe = line.substr(semicolon + 1);
        // Trim leading whitespace from inline recipe
        auto rs = inline_recipe.find_first_not_of(" \t");
        if (rs != std::string::npos) {
            inline_recipe = inline_recipe.substr(rs);
        }
    } 
    else {
        prereqs_str = line.substr(prereq_start);
    }

    // Split prerequisites, handling order-only after '|'
    size_t pipe_pos = std::string::npos;
    paren_depth = 0;
    for (size_t i = 0; i < prereqs_str.size(); ++i) {
        char c = prereqs_str[i];
        if (c == '$' && i + 1 < prereqs_str.size() &&
            (prereqs_str[i + 1] == '(' || prereqs_str[i + 1] == '{')) {
            paren_depth++;
            i++;
        } 
        else if ((c == ')' || c == '}') && paren_depth > 0) {
            paren_depth--;
        } 
        else if (c == '|' && paren_depth == 0) {
            pipe_pos = i;
            break;
        }
    }

    if (pipe_pos != std::string::npos) {
        rule.normal_prereqs = split_words(prereqs_str.substr(0, pipe_pos));
        rule.order_only_prereqs = split_words(prereqs_str.substr(pipe_pos + 1));
    } 
    else {
        rule.normal_prereqs = split_words(prereqs_str);
    }

    // Collect inline recipe
    if (!inline_recipe.empty()) {
        rule.recipes.push_back(inline_recipe);
    }

    // Collect subsequent recipe lines
    while (index + 1 < lines.size() &&
           lines[index + 1].kind == LogicalLine::Recipe) {
        index++;
        rule.recipes.push_back(lines[index].content);
    }

    return rule;
}

ast::VariableAssignment Parser::parse_assignment(const std::string& line,
                                                  const SourceLocation& loc) {
    ast::VariableAssignment assignment;
    assignment.loc = loc;

    // Check for 'override' prefix
    std::string work = line;
    if (work.starts_with("override ") || work.starts_with("override\t")) {
        assignment.is_override = true;
        work = work.substr(9);
        auto s = work.find_first_not_of(" \t");
        if (s != std::string::npos) work = work.substr(s);
    }

    // Check for 'export' prefix
    if (work.starts_with("export ") || work.starts_with("export\t")) {
        assignment.is_export = true;
        work = work.substr(7);
        auto s = work.find_first_not_of(" \t");
        if (s != std::string::npos) work = work.substr(s);
    }

    // Find the assignment operator
    size_t op_pos = find_assignment_op(work, assignment.flavor);
    if (op_pos == std::string::npos) {
        throw ParseError("missing separator", loc);
    }

    // Extract variable name (trim trailing whitespace)
    std::string name = work.substr(0, op_pos);
    auto end = name.find_last_not_of(" \t");
    if (end != std::string::npos) {
        name = name.substr(0, end + 1);
    }

    assignment.name = name;

    // Extract value (skip operator, trim leading whitespace)
    size_t val_start = op_pos + 1;
    if (assignment.flavor == ast::VariableAssignment::Simple ||
        assignment.flavor == ast::VariableAssignment::Append ||
        assignment.flavor == ast::VariableAssignment::Conditional) {
        val_start = op_pos + 2; // skip := or += or ?=
    }
    std::string value;
    if (val_start < work.size()) {
        value = work.substr(val_start);
        auto s = value.find_first_not_of(" \t");
        if (s != std::string::npos) {
            value = value.substr(s);
        } 
        else {
            value.clear();
        }
    }
    assignment.value = value;

    return assignment;
}

size_t Parser::find_assignment_op(const std::string& line,
                                   ast::VariableAssignment::Flavor& flavor) {
    int paren_depth = 0;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '$' && i + 1 < line.size() &&
            (line[i + 1] == '(' || line[i + 1] == '{')) {
            ++paren_depth;
            ++i;
        } 
        else if ((c == ')' || c == '}') && paren_depth > 0) {
            --paren_depth;
        } 
        else if (paren_depth == 0) {
            if (c == ':' && i + 1 < line.size() && line[i + 1] == '=') {
                flavor = ast::VariableAssignment::Simple;
                return i;
            }
            if (c == '+' && i + 1 < line.size() && line[i + 1] == '=') {
                flavor = ast::VariableAssignment::Append;
                return i;
            }
            if (c == '?' && i + 1 < line.size() && line[i + 1] == '=') {
                flavor = ast::VariableAssignment::Conditional;
                return i;
            }
            if (c == '=') {
                flavor = ast::VariableAssignment::Recursive;
                return i;
            }
        }
    }
    return std::string::npos;
}

std::vector<std::string> Parser::split_words(const std::string& text) {
    std::vector<std::string> words;
    std::string current;
    int paren_depth = 0;

    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];

        if (c == '$' && i + 1 < text.size() &&
            (text[i + 1] == '(' || text[i + 1] == '{')) {
            current += c;
            current += text[i + 1];
            ++paren_depth;
            ++i;
            continue;
        }

        if ((c == ')' || c == '}') && paren_depth > 0) {
            paren_depth--;
            current += c;
            continue;
        }

        if ((c == ' ' || c == '\t') && paren_depth == 0) {
            if (!current.empty()) {
                words.push_back(current);
                current.clear();
            }
            
            continue;
        }

        current += c;
    }

    if (!current.empty()) {
        words.push_back(current);
    }

    return words;
}

} // namespace mymake
