#include <stack>
#include <iostream>
#include <limits>
#include <stdexcept>

void move_disk(std::stack<std::size_t>& from, std::stack<std::size_t>& to,
               char from_rod, char to_rod) {
    if (from.empty() && to.empty()) {
        return;
    }
    
    auto disk_from = from.empty() ? std::numeric_limits<std::size_t>::max() : from.top();
    auto disk_to   = to.empty()   ? std::numeric_limits<std::size_t>::max() : to.top();
    if (disk_from < disk_to) {
        to.push(disk_from);
        from.pop();
    } 
    else {
        from.push(disk_to);
        to.pop();
    }
}

namespace recursive {

void hanoi_tower_recursive(std::size_t n,
                           std::stack<std::size_t>& source,
                           std::stack<std::size_t>& destination,
                           std::stack<std::size_t>& auxiliary,
                           char from_rod = 'A', char to_rod = 'B', char aux_rod = 'C') {
    if (n == 1) {
        ::move_disk(source, destination, from_rod, to_rod);
        return;
    }
    hanoi_tower_recursive(n - 1, source, auxiliary, destination,
                          from_rod, aux_rod, to_rod);
    ::move_disk(source, destination, from_rod, to_rod);
    hanoi_tower_recursive(n - 1, auxiliary, destination, source,
                          aux_rod, to_rod, from_rod);
}

void hanoi_tower(std::stack<std::size_t>& source,
                 std::stack<std::size_t>& destination,
                 char from_rod = 'A', char to_rod = 'B', char aux_rod = 'C') {
    if (source.empty()) {
        throw std::runtime_error("hanoi_tower: source must be non-empty");
    }
    
    auto n = source.size();
    std::stack<std::size_t> auxiliary;
    while (!destination.empty()) { 
        destination.pop(); 
    }
    
    hanoi_tower_recursive(n, source, destination, auxiliary,
                          from_rod, to_rod, aux_rod);
}

}

namespace iterative {

void hanoi_tower(std::size_t n, std::stack<std::size_t>& source, std::stack<std::size_t>& destination, 
                 char from_rod = 'A', char to_rod = 'B', char aux_rod = 'C') {
    if (source.empty()) {
        throw std::runtime_error("hanoi_tower: source must be non-empty");
    }
    
    std::stack<std::size_t> auxiliary;
    while (!source.empty()) {
        source.pop();
    }
    while (!destination.empty()) {
        destination.pop();
    }
    
    for (auto i = n; i >= 0; --i) {
        source.push(i);
    }
    
    auto total_moves = (1 << n) - 1; // time complexity of the algorithm is 2 ^ n, because it does 2 ^ n - 1 moves
    char actual_to = to_rod;
    char actual_aux = aux_rod;
    if ((n & 1) == 0) {
        std::swap(actual_to, actual_aux);
    }
    
    for (auto i = 1; i < total_moves + 1; ++i) {
        if (i % 3 == 0) {
            ::move_disk(auxiliary, destination, actual_aux, actual_to);
        }
        else if (i % 3 == 1) {
            ::move_disk(source, destination, from_rod, actual_to);
        }
        else {
            ::move_disk(source, auxiliary, from_rod, actual_aux);
        }
    }
}

void hanoi_tower(std::stack<std::size_t>& source,
                 std::stack<std::size_t>& destination,
                 char from_rod = 'A', char to_rod = 'B', char aux_rod = 'C') {
    if (source.empty()) {
        throw std::runtime_error("hanoi_tower: source must be non-empty");
    }

    const std::size_t n = source.size();
    std::stack<std::size_t> auxiliary;
    while (!destination.empty()) { 
        destination.pop(); 
    }

    const std::size_t total_moves = (1ull << n) - 1;
    char A = from_rod;
    char B = to_rod;
    char C = aux_rod;
    if (n % 2 == 0) {
        std::swap(B, C);
    }

    for (std::size_t i = 1; i <= total_moves; ++i) {
        if (i % 3 == 1) {
            ::move_disk(source, destination, A, B);
        } 
        else if (i % 3 == 2) {
            ::move_disk(source, auxiliary, A, C);
        } 
        else {
            ::move_disk(auxiliary, destination, C, B);
        }
    }
}

}
