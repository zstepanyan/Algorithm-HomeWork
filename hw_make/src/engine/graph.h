#ifndef MYMAKE_ENGINE_GRAPH_H
#define MYMAKE_ENGINE_GRAPH_H

#include "engine/rule_database.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace mymake {

struct GraphNode {
    std::string target;
    std::vector<std::string> normal_prereqs;
    std::vector<std::string> order_only_prereqs;
    std::vector<std::string> recipes;
    std::string stem; // for pattern rules: the % match
    bool is_phony = false;
};

class DependencyGraph {
public:
    void build(const std::vector<std::string>& goals,
               const RuleDatabase& rules);

    // Returns targets in topological order (dependencies before dependents).
    std::vector<std::string> topological_order() const;

    const GraphNode* node(const std::string& target) const;
    bool has_node(const std::string& target) const;

private:
    enum class Color { White, Gray, Black };

    void add_target(const std::string& target, const RuleDatabase& rules);

    void topo_sort(const std::string& target,
                   std::unordered_map<std::string, Color>& colors,
                   std::vector<std::string>& order) const;

    std::unordered_map<std::string, GraphNode> nodes_;
};

} // namespace mymake

#endif // MYMAKE_ENGINE_GRAPH_H
