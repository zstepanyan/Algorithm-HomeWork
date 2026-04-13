#ifndef MYMAKE_ENGINE_RULE_DATABASE_H
#define MYMAKE_ENGINE_RULE_DATABASE_H

#include "core/source_location.h"
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace mymake {

struct ExplicitRule {
    std::string target;
    std::vector<std::string> normal_prereqs;
    std::vector<std::string> order_only_prereqs;
    std::vector<std::string> recipes;
    bool is_double_colon = false;
    SourceLocation loc;
};

struct PatternRule {
    std::string target_pattern;  // e.g. "%.o"
    std::vector<std::string> prereq_patterns; // e.g. {"%.c"}
    std::vector<std::string> recipes;
    SourceLocation loc;
};

class RuleDatabase {
public:
    void add_explicit_rule(ExplicitRule rule);
    void add_pattern_rule(PatternRule rule);

    const std::vector<ExplicitRule>* find_explicit(const std::string& target) const;
    const PatternRule* find_matching_pattern(const std::string& target) const;

    // Match a target against a pattern (e.g. "foo.o" against "%.o").
    // Returns the stem (e.g. "foo") or nullopt if no match.
    static std::optional<std::string> match_pattern(const std::string& target,
                                                     const std::string& pattern);

    // Substitute stem into a pattern (e.g. "foo" into "%.c" -> "foo.c").
    static std::string substitute_pattern(const std::string& pattern,
                                           const std::string& stem);

    std::unordered_set<std::string> phony_targets;
    std::optional<std::string> default_goal;

private:
    // target -> list of rules (multiple for double-colon)
    std::unordered_map<std::string, std::vector<ExplicitRule>> explicit_rules_;
    std::vector<PatternRule> pattern_rules_;
};

} // namespace mymake

#endif // MYMAKE_ENGINE_RULE_DATABASE_H
