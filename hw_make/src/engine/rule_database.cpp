#include "engine/rule_database.h"

namespace mymake {

void RuleDatabase::add_explicit_rule(ExplicitRule rule) {
    const std::string& target = rule.target;

    // Track default goal: first non-pattern, non-special target
    if (!default_goal.has_value() &&
        target[0] != '.' && target[0] != '%') {
        default_goal = target;
    }

    // Handle .PHONY
    if (target == ".PHONY") {
        for (const auto& prereq : rule.normal_prereqs) {
            phony_targets.insert(prereq);
        }

        return;
    }

    auto& rules = explicit_rules_[target];
    if (rule.is_double_colon) {
        rules.push_back(std::move(rule));
    } 
    else {
        // Single-colon: merge prerequisites, replace recipes if new ones given
        if (rules.empty()) {
            rules.push_back(std::move(rule));
        } 
        else {
            auto& existing = rules[0];
            for (auto& p : rule.normal_prereqs) {
                existing.normal_prereqs.push_back(std::move(p));
            }
            for (auto& p : rule.order_only_prereqs) {
                existing.order_only_prereqs.push_back(std::move(p));
            }
            if (!rule.recipes.empty()) {
                existing.recipes = std::move(rule.recipes);
                existing.loc = rule.loc;
            }
        }
    }
}

void RuleDatabase::add_pattern_rule(PatternRule rule) {
    pattern_rules_.push_back(std::move(rule));
}

const std::vector<ExplicitRule>*
RuleDatabase::find_explicit(const std::string& target) const {
    auto it = explicit_rules_.find(target);
    if (it != explicit_rules_.end()) {
        return &it->second;
    }
    
    return nullptr;
}

const PatternRule*
RuleDatabase::find_matching_pattern(const std::string& target) const {
    for (const auto& pattern : pattern_rules_) {
        if (match_pattern(target, pattern.target_pattern).has_value()) {
            return &pattern;
        }
    }
    return nullptr;
}

std::optional<std::string>
RuleDatabase::match_pattern(const std::string& target,
                             const std::string& pattern) {
    auto pct = pattern.find('%');
    if (pct == std::string::npos) {
        // Exact match only
        if (target == pattern) return std::string{};
        return std::nullopt;
    }

    std::string prefix = pattern.substr(0, pct);
    std::string suffix = pattern.substr(pct + 1);

    if (target.size() < prefix.size() + suffix.size()) {
        return std::nullopt;
    }

    if (target.substr(0, prefix.size()) != prefix) {
        return std::nullopt;
    }

    if (!suffix.empty() &&
        target.substr(target.size() - suffix.size()) != suffix) {
        return std::nullopt;
    }

    return target.substr(prefix.size(),
                         target.size() - prefix.size() - suffix.size());
}

std::string RuleDatabase::substitute_pattern(const std::string& pattern,
                                              const std::string& stem) {
    auto pct = pattern.find('%');
    if (pct == std::string::npos) {
        return pattern;
    }
    
    return pattern.substr(0, pct) + stem + pattern.substr(pct + 1);
}

} // namespace mymake
