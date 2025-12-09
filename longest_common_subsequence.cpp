#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>

namespace recursive {

struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

using Memo = std::unordered_map<
    std::pair<std::size_t, std::size_t>,
    unsigned,
    PairHash
>;

unsigned recursive_lcs(std::size_t i, std::size_t j,
                       const std::string& a, const std::string& b,
                       Memo& memo) {
    if (i == 0 || j == 0) {
        return 0;
    }
    
    auto key = std::make_pair(i, j);
    if (memo.find(key) != memo.end()) {
        return memo[key];
    }
    
    unsigned res;
    if (a[i - 1] == b[j - 1]) {
        res = recursive_lcs(i - 1, j - 1, a, b, memo) + 1;
    } 
    else {
        res = std::max(
            recursive_lcs(i - 1, j, a, b, memo),
            recursive_lcs(i, j - 1, a, b, memo)
        );
    }

    memo[key] = res;
    return res;
}

unsigned length_of_lcs(const std::string& a, const std::string& b) {
    Memo memo;
    return recursive_lcs(a.size(), b.size(), a, b, memo);
}

std::string reconstruct_lcs(const std::string& a, const std::string& b) {
    Memo memo;
    recursive_lcs(a.size(), b.size(), a, b, memo);

    std::string lcs;
    std::size_t i = a.size(), j = b.size();
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            lcs = a[i - 1] + lcs;
            --i; --j;
        }
        else if (memo[std::make_pair(i - 1, j)] >= memo[std::make_pair(i, j - 1)]) {
            --i;
        }
        else {
            --j;
        }
    }

    return lcs;
}

} // namespace recursive

namespace iterative {

unsigned length_of_lcs(const std::string& a, const std::string& b) {
    std::size_t n = a.length();
    std::size_t m = b.length();

    std::vector<std::vector<unsigned>> dp(n + 1, std::vector<unsigned>(m + 1, 0));
    for (std::size_t i = 1; i <= n; ++i) {
        for (std::size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } 
            else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    return dp[n][m];
}

std::string reconstruction_of_lcs(const std::string& a, const std::string& b) {
    std::size_t n = a.length();
    std::size_t m = b.length();

    std::vector<std::vector<unsigned>> dp(n + 1, std::vector<unsigned>(m + 1, 0));
    for (std::size_t i = 1; i <= n; ++i) {
        for (std::size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } 
            else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    std::string ans;
    std::size_t i = n, j = m;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {           
            ans = a[i - 1] + ans;
            --i;
            --j;
        }
        else if (dp[i - 1][j] > dp[i][j - 1]) {
            --i;
        }
        else {
            --j;
        }
    }

    return ans;
}

unsigned space_optimized_lcs(const std::string& a, const std::string& b) {
    if (a.length() > b.length()) {
        return space_optimized_lcs(b, a);
    }

    std::vector<unsigned> prev(b.size() + 1, 0);
    std::vector<unsigned> curr(b.size() + 1, 0);  
    for (char c : a) {
        for (std::size_t j = 1; j <= b.size(); ++j) {
            if (c == b[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } 
            else {
                curr[j] = std::max(prev[j], curr[j - 1]);
            }
        }

        std::swap(prev, curr);
    }
    
    return prev[b.size()];
}

} // namespace iterative

