#include "engine/executor.h"
#include "core/errors.h"
#include <cstdlib>
#include <iostream>

namespace mymake {

Executor::Executor(const Config& config, VariableTable& vars)
    : config_(config), vars_(vars) {}

int Executor::execute(const DependencyGraph& graph,
                       const std::vector<std::string>& goals) {
    auto order = graph.topological_order();

    for (const auto& target : order) {
        const auto* n = graph.node(target);
        if (!n) continue;

        if (!needs_rebuild(*n)) {
            continue;
        }

        if (n->recipes.empty()) {
            // No recipes — nothing to do (might be a phony grouping target
            // or a source file that must exist)
            if (!n->is_phony && !std::filesystem::exists(target)) {
                // Check if this target is a goal or has a rule
                bool is_goal = false;
                for (const auto& g : goals) {
                    if (g == target) { is_goal = true; break; }
                }
                if (is_goal) {
                    std::cerr << "my_make: *** No rule to make target '"
                              << target << "'.  Stop." << std::endl;
                    return 2;
                }
                // It's a prerequisite without a rule — it must be an existing file
                if (!n->normal_prereqs.empty() || !n->order_only_prereqs.empty()) {
                    // Has prereqs, so it's a real rule, just no recipes
                } 
                else {
                    std::cerr << "my_make: *** No rule to make target '"
                              << target << "'.  Stop." << std::endl;
                    return 2;
                }
            }

            continue;
        }

        int rc = run_recipes(*n);
        if (rc != 0 && !config_.keep_going) {
            return rc;
        }
    }

    return 0;
}

bool Executor::needs_rebuild(const GraphNode& node) const {
    if (node.is_phony) return true;
    if (config_.always_make) return true;

    auto target_mtime = get_mtime(node.target);
    if (!target_mtime) {
        return true; // target doesn't exist
    }

    for (const auto& prereq : node.normal_prereqs) {
        auto prereq_mtime = get_mtime(prereq);
        if (prereq_mtime && *prereq_mtime > *target_mtime) {
            return true;
        }
    }
    // order-only prereqs only matter for existence, not timestamps

    return false;
}

int Executor::run_recipes(const GraphNode& node) {
    // Set up automatic variables
    vars_.push_scope();
    vars_.set_local("@", node.target);

    if (!node.normal_prereqs.empty()) {
        vars_.set_local("<", node.normal_prereqs[0]);

        std::string all_prereqs;
        for (const auto& p : node.normal_prereqs) {
            if (!all_prereqs.empty()) all_prereqs += ' ';
            all_prereqs += p;
        }
        vars_.set_local("^", all_prereqs);
    }

    if (!node.stem.empty()) {
        vars_.set_local("*", node.stem);
    }

    int result = 0;
    for (const auto& recipe : node.recipes) {
        if (recipe.empty()) continue;

        // Parse recipe prefixes: @ (silent), - (ignore error), + (force)
        std::string cmd = recipe;
        bool silent = config_.silent;
        bool ignore_error = false;

        while (!cmd.empty()) {
            if (cmd[0] == '@') {
                silent = true;
                cmd = cmd.substr(1);
            } 
            else if (cmd[0] == '-') {
                ignore_error = true;
                cmd = cmd.substr(1);
            } 
            else if (cmd[0] == '+') {
                cmd = cmd.substr(1);
            } 
            else {
                break;
            }
        }

        // Trim leading whitespace
        auto s = cmd.find_first_not_of(" \t");
        if (s != std::string::npos) {
            cmd = cmd.substr(s);
        }

        if (cmd.empty()) continue;

        // TODO: expand variables in cmd (Phase 2)

        result = run_command(cmd, silent, ignore_error);
        if (result != 0 && !ignore_error) {
            vars_.pop_scope();
            std::cerr << "my_make: *** [" << node.target
                      << "] Error " << result << std::endl;

            return result;
        }
    }

    vars_.pop_scope();
    return 0;
}

int Executor::run_command(const std::string& command,
                           bool silent, bool ignore_error) {
    if (!silent) {
        std::cout << command << std::endl;
    }

    if (config_.dry_run) {
        return 0;
    }

    int status = std::system(command.c_str());

    if (status == -1) {
        return 127; // could not launch shell
    }

    // Extract exit code from system() return value
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    return 1;
}

std::optional<std::filesystem::file_time_type>
Executor::get_mtime(const std::string& path) const {
    auto it = mtime_cache_.find(path);
    if (it != mtime_cache_.end()) {
        return it->second;
    }

    std::error_code ec;
    auto mtime = std::filesystem::last_write_time(path, ec);
    if (ec) {
        mtime_cache_[path] = std::nullopt;
        return std::nullopt;
    }

    mtime_cache_[path] = mtime;
    return mtime;
}

} // namespace mymake
