#include <vector>
#include <iostream>
#include <string>
#include <climits>
#include <limits>
#include <type_traits>
#include <stdexcept>

void optimal_parenthesis_print(const std::vector<std::vector<long long>>& s, std::size_t i, std::size_t j) {
    if (i == j) {
        std::cout << "A" << i;
    } 
    else {
        std::cout << "(";
        optimal_parenthesis_print(s, i, s[i][j]);
        optimal_parenthesis_print(s, s[i][j] + 1, j);
        std::cout << ")";
    }
}

/*
    p = {1, 2, 3, 4} =>:
        dim(A1) = 1 x 2
        dim(A2) = 2 x 3
        dim(A3) = 3 x 4
*/
std::pair<long long, std::vector<std::vector<long long>>> matrix_chain_order(const std::vector<long long>& p) {
    std::size_t n = p.size() - 1; // number of matrices
    if (n == 0) {
        throw std::invalid_argument("At least one matrix required.");
    }

    std::vector<std::vector<long long>> m(n + 1, std::vector<long long>(n + 1, 0));
    std::vector<std::vector<long long>> s(n + 1, std::vector<long long>(n + 1, 0));

    for (std::size_t chain_len = 2; chain_len <= n; ++chain_len) {
        for (std::size_t i = 1; i <= n - chain_len + 1; ++i) {
            std::size_t j = i + chain_len - 1;
            m[i][j] = std::numeric_limits<long long>::max();
            for (std::size_t k = i; k < j; ++k) {
                long long cost = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (cost < m[i][j]) {
                    m[i][j] = cost;
                    s[i][j] = static_cast<long long>(k); // safe if k fits in long long
                }
            }
        }
    }

    return {m[1][n], s};
}
