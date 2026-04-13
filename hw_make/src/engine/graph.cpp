#include "engine/graph.h"
#include "core/errors.h"

namespace mymake {

void DependencyGraph::build(const std::vector<std::string>& goals,
                             const RuleDatabase& rules) {
    for (const auto& goal : goals) {
        add_target(goal, rules);
    }
}

void DependencyGraph::add_target(const std::string& target,
                                  const RuleDatabase& rules) {
    if (nodes_.count(target)) {
        return; // already visited
    }

    GraphNode node;
    node.target = target;
    node.is_phony = rules.phony_targets.count(target) > 0;

    // Try explicit rules first
    const auto* explicit_rules = rules.find_explicit(target);
    if (explicit_rules && !explicit_rules->empty()) {
        const auto& rule = (*explicit_rules)[0];
        node.normal_prereqs = rule.normal_prereqs;
        node.order_only_prereqs = rule.order_only_prereqs;
        node.recipes = rule.recipes;
    } 
    else {
        // Try pattern rules
        const auto* pattern = rules.find_matching_pattern(target);
        if (pattern) {
            auto stem = RuleDatabase::match_pattern(target,
                                                     pattern->target_pattern);
            if (stem) {
                node.stem = *stem;
                node.recipes = pattern->recipes;
                for (const auto& prereq_pat : pattern->prereq_patterns) {
                    node.normal_prereqs.push_back(
                        RuleDatabase::substitute_pattern(prereq_pat, *stem));
                }
            }
        }
        // If no rule found, the target must be an existing file
        // (checked at execution time)
    }

    nodes_[target] = std::move(node);

    // Recursively add prerequisites
    const auto& added = nodes_[target];
    for (const auto& prereq : added.normal_prereqs) {
        add_target(prereq, rules);
    }
    for (const auto& prereq : added.order_only_prereqs) {
        add_target(prereq, rules);
    }
}

std::vector<std::string> DependencyGraph::topological_order() const {
    std::unordered_map<std::string, Color> colors;
    for (const auto& [name, _] : nodes_) {
        colors[name] = Color::White;
    }

    std::vector<std::string> order;
    for (const auto& [name, _] : nodes_) {
        if (colors[name] == Color::White) {
            topo_sort(name, colors, order);
        }
    }

    return order;
}

void DependencyGraph::topo_sort(
    const std::string& target,
    std::unordered_map<std::string, Color>& colors,
    std::vector<std::string>& order) const {

    colors[target] = Color::Gray;

    auto it = nodes_.find(target);
    if (it != nodes_.end()) {
        const auto& node = it->second;

        auto visit = [&](const std::string& prereq) {
            auto cit = colors.find(prereq);
            if (cit == colors.end()) return; // not in graph (leaf file)
            if (cit->second == Color::Gray) {
                throw CycleError("Circular " + target + " <- " + prereq +
                                 " dependency dropped");
            }
            if (cit->second == Color::White) {
                topo_sort(prereq, colors, order);
            }
        };

        for (const auto& prereq : node.normal_prereqs) {
            visit(prereq);
        }
        for (const auto& prereq : node.order_only_prereqs) {
            visit(prereq);
        }
    }

    colors[target] = Color::Black;
    order.push_back(target);
}

const GraphNode* DependencyGraph::node(const std::string& target) const {
    auto it = nodes_.find(target);
    if (it != nodes_.end()) {
        return &it->second;
    }
    
    return nullptr;
}

bool DependencyGraph::has_node(const std::string& target) const {
    return nodes_.count(target) > 0;
}

} // namespace mymake
