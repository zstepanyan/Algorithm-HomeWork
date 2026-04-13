#include "cli/cli.h"
#include "core/errors.h"
#include "engine/executor.h"
#include "engine/graph.h"
#include "engine/rule_database.h"
#include "eval/variable_table.h"
#include "parser/lexer.h"
#include "parser/parser.h"

#include <filesystem>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        auto config = mymake::parse_args(argc, argv);

        // Change directory if requested
        if (!config.directory.empty()) {
            std::filesystem::current_path(config.directory);
        }

        // Set up variable table with command-line overrides
        mymake::VariableTable vars;
        for (const auto& [name, value] : config.overrides) {
            vars.set(name, value, mymake::VarFlavor::Simple,
                     mymake::VarOrigin::CommandLine);
        }

        // Lex
        mymake::Lexer lexer;
        auto lines = lexer.lex_file(config.makefile_path);

        // Parse
        mymake::Parser parser;
        auto makefile = parser.parse(lines);

        // Evaluate: walk AST and populate rule database
        mymake::RuleDatabase rules;
        for (auto& node : makefile) {
            if (auto* rule = std::get_if<mymake::ast::Rule>(&node)) {
                if (rule->is_pattern) {
                    // Pattern rule
                    for (const auto& target : rule->targets) {
                        mymake::PatternRule prule;
                        prule.target_pattern = target;
                        prule.prereq_patterns = rule->normal_prereqs;
                        prule.recipes = rule->recipes;
                        prule.loc = rule->loc;
                        rules.add_pattern_rule(std::move(prule));
                    }
                } 
                else {
                    // Explicit rule
                    for (const auto& target : rule->targets) {
                        mymake::ExplicitRule erule;
                        erule.target = target;
                        erule.normal_prereqs = rule->normal_prereqs;
                        erule.order_only_prereqs = rule->order_only_prereqs;
                        erule.recipes = rule->recipes;
                        erule.is_double_colon = rule->is_double_colon;
                        erule.loc = rule->loc;
                        rules.add_explicit_rule(std::move(erule));
                    }
                }
            } 
            else if (auto* assign = std::get_if<mymake::ast::VariableAssignment>(&node)) {
                // Phase 1: basic variable storage (no expansion yet)
                mymake::VarFlavor flavor = (assign->flavor == mymake::ast::VariableAssignment::Simple)
                    ? mymake::VarFlavor::Simple : mymake::VarFlavor::Recursive;
                mymake::VarOrigin origin = assign->is_override
                    ? mymake::VarOrigin::Override : mymake::VarOrigin::File;

                switch (assign->flavor) {
                case mymake::ast::VariableAssignment::Append:
                    vars.append(assign->name, assign->value);
                    break;
                case mymake::ast::VariableAssignment::Conditional:
                    vars.set_conditional(assign->name, assign->value,
                                         flavor, origin);
                    break;
                default:
                    vars.set(assign->name, assign->value, flavor, origin);
                    break;
                }
            }
        }

        // Determine goals
        std::vector<std::string> goals = config.goals;
        if (goals.empty()) {
            if (rules.default_goal) {
                goals.push_back(*rules.default_goal);
            } 
            else {
                std::cerr << "my_make: *** No targets.  Stop." << std::endl;
                return 2;
            }
        }

        // Build dependency graph
        mymake::DependencyGraph graph;
        graph.build(goals, rules);

        // Execute
        mymake::Executor executor(config, vars);
        return executor.execute(graph, goals);

    } catch (const mymake::MakeError& e) {
        std::cerr << mymake::format_error(e) << std::endl;
        return 2;
    } catch (const std::exception& e) {
        std::cerr << "my_make: *** " << e.what() << std::endl;
        return 2;
    }
}
