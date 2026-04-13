#include "eval/variable_table.h"

namespace mymake {

VariableTable::VariableTable() {
    // Start with one global scope
    scopes_.emplace_back();
}

void VariableTable::set(const std::string& name, const std::string& value,
                         VarFlavor flavor, VarOrigin origin) {
    auto& global = scopes_[0];
    auto it = global.find(name);
    if (it != global.end() && !can_override(it->second, origin)) {
        return;
    }
   
    global[name] = Variable{value, flavor, origin, false};
}

void VariableTable::append(const std::string& name, const std::string& value) {
    auto& global = scopes_[0];
    auto it = global.find(name);
    if (it != global.end()) {
        if (it->second.value.empty()) {
            it->second.value = value;
        } 
        else {
            it->second.value += " " + value;
        }
    } 
    else {
        global[name] = Variable{value, VarFlavor::Recursive, VarOrigin::File, false};
    }
}

void VariableTable::set_conditional(const std::string& name,
                                     const std::string& value,
                                     VarFlavor flavor, VarOrigin origin) {
    if (!is_defined(name)) {
        set(name, value, flavor, origin);
    }
}

std::optional<std::string> VariableTable::lookup(const std::string& name) const {
    // Search from innermost scope outward
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return found->second.value;
        }
    }
    
    return std::nullopt;
}

bool VariableTable::is_defined(const std::string& name) const {
    return lookup(name).has_value();
}

void VariableTable::push_scope() {
    scopes_.emplace_back();
}

void VariableTable::pop_scope() {
    if (scopes_.size() > 1) {
        scopes_.pop_back();
    }
}

void VariableTable::set_local(const std::string& name,
                               const std::string& value) {
    scopes_.back()[name] = Variable{value, VarFlavor::Simple,
                                     VarOrigin::Automatic, false};
}

bool VariableTable::can_override(const Variable& existing,
                                  VarOrigin new_origin) const {
    // Command-line overrides everything except override directive
    // Override directive overrides everything
    // File-level can't override command-line
    if (existing.origin == VarOrigin::CommandLine &&
        new_origin != VarOrigin::Override &&
        new_origin != VarOrigin::CommandLine) {
   
            return false;
    }
   
    return true;
}

} // namespace mymake
