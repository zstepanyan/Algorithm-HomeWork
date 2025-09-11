#include <cmath>
#include <vector>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <type_traits>

template <typename T>
std::size_t fibRecursive(const T& n) {
    if constexpr (!std::is_integral<T>::value) {
        throw std::invalid_argument("fibRecursive: Argument type must be integral.");
    }

    if (n < 0) {
        throw std::invalid_argument("fibRecursive: Fibonacci sequence is only defined for non-negative integers.");
    }
    if (n <= 1) {
        return n;
    }

    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

template <typename T>
std::size_t fibIterative(const T& n) {
    if constexpr (!std::is_integral<T>::value) {
        throw std::invalid_argument("fibIterative: Argument type must be integral.");
    }

    if (n < 0) {
        throw std::invalid_argument("fibIterative: Fibonacci sequence is only defined for non-negative integers.");
    }
    if (n <= 1) {
        return n;
    }

    std::size_t a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        std::size_t next = a + b;
        a = b;
        b = next;
    }

    return b;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
std::size_t fibDp(const T& n) {
    if (n < 0) {
        throw std::invalid_argument("fibDp: Fibonacci sequence is only defined for non-negative integers.");
    }

    if (n <= 1) {
        return n;
    }

    std::vector<std::size_t> dp{0, 1};
    for (std::size_t i = 2; i <= n; ++i) {
        dp.push_back(dp[i - 1] + dp[i - 2]);
    }

    return dp.back();
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
std::size_t fibBinet(const T& n) {
    if (n < 0) {
        throw std::invalid_argument("fibBinet: Fibonacci sequence is only defined for non-negative integers.");
    }

    const long double phi = (1 + std::sqrt(5.0L)) / 2.0L;
    const long double psi = (1 - std::sqrt(5.0L)) / 2.0L;
    long double value = (std::pow(phi, n) - std::pow(psi, n)) / std::sqrt(5.0L) + 0.5L;

    return static_cast<std::size_t>(std::round(value));
}

std::vector<std::vector<std::size_t>> matrixMultiply(const std::vector<std::vector<std::size_t>>& A,
                                                     const std::vector<std::vector<std::size_t>>& B) {
    return {
        {
            A[0][0] * B[0][0] + A[0][1] * B[1][0],
            A[0][0] * B[0][1] + A[0][1] * B[1][1]
        },
        {
            A[1][0] * B[0][0] + A[1][1] * B[1][0],
            A[1][0] * B[0][1] + A[1][1] * B[1][1]
        }
    };
}

std::vector<std::vector<std::size_t>> matrixPower(std::vector<std::vector<std::size_t>> base, std::size_t exp) {
    std::vector<std::vector<std::size_t>> result{{1, 0}, {0, 1}};

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = matrixMultiply(result, base);
        }

        base = matrixMultiply(base, base);
        exp /= 2;
    }

    return result;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
std::size_t fibMatrix(const T& n) {
    if (n < 0) {
        throw std::invalid_argument("fibMatrix: Fibonacci sequence is only defined for non-negative integers.");
    }

    if (n <= 1) {
        return n;
    }

    std::vector<std::vector<std::size_t>> A{{1, 1}, {1, 0}};
    auto result = matrixPower(A, n - 1);

    return result[0][0];
}
