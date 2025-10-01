#include <stack>
#include <string>

bool isPalindrome(const std::string& s) {
    if (s.length() == 0 || s.length() == 1) {
        return true;
    }

    std::stack<char> stk;
    std::size_t mid = s.length() / 2;
    for (std::size_t i = 0; i < mid; ++i) {
        stk.push(s[i]);
    }

    std::size_t pos = (s.length() % 2 == 0) ? mid : mid + 1; 
    for (std::size_t i = pos; i < s.length(); ++i) {
        if (s[i] != stk.top()) {
            return false;
        }

        stk.pop();
    }

    return stk.empty(); 
}