#ifndef MYMAKE_EVAL_VARIABLE_TABLE_H
#define MYMAKE_EVAL_VARIABLE_TABLE_H

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace mymake {

enum class VarOrigin { Default, Environment, File, Override, CommandLine, Automatic };
enum class VarFlavor { Recursive, Simple };

struct Variable {
    std::string value;
    VarFlavor flavor = VarFlavor::Recursive;
    VarOrigin origin = VarOrigin::File;
    bool is_export = false;
};

class VariableTable {
public:
    VariableTable();

    void set(const std::string& name, const std::string& value,
             VarFlavor flavor, VarOrigin origin);
    void append(const std::string& name, const std::string& value);
    void set_conditional(const std::string& name, const std::string& value,
                         VarFlavor flavor, VarOrigin origin);

    // Simple lookup — returns the raw (unexpanded) value.
    std::optional<std::string> lookup(const std::string& name) const;

    bool is_defined(const std::string& name) const;

    // Push/pop scope for automatic variables ($@, $<, etc.)
    void push_scope();
    void pop_scope();

    // Set a variable in the current (innermost) scope only.
    void set_local(const std::string& name, const std::string& value);

private:
    // scopes_[0] = global. scopes_.back() = current innermost.
    std::vector<std::unordered_map<std::string, Variable>> scopes_;

    // Check if origin allows overwriting existing variable.
    bool can_override(const Variable& existing, VarOrigin new_origin) const;
};

} // namespace mymake

#endif // MYMAKE_EVAL_VARIABLE_TABLE_H
