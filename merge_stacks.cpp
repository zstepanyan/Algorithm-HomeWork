#include <stack>
#include <type_traits>
#include <cassert>

template <typename T>
std::stack<T> merge_stacks(const std::stack<T>& stack_a, const std::stack<T>& stack_b) {
    static_assert(std::is_arithmetic_v<T>, "merge_stacks: stack value type must be arithmetic");

    std::stack<T> stk1 = stack_a;
    std::stack<T> stk2 = stack_b;
    std::stack<T> ans;
    
    while (!stk1.empty() && !stk2.empty()) {
        if (stk1.top() >= stk2.top()) {
            ans.push(stk1.top());
            stk1.pop();
        }
        else {
            ans.push(stk2.top());
            stk2.pop();
        }
    }

    while (!stk1.empty()) {
        ans.push(stk1.top());
        stk1.pop();
    }

    while (!stk2.empty()) {
        ans.push(stk2.top());
        stk2.pop();
    }

    return ans;
}
