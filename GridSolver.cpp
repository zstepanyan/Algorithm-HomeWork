#include <vector>
#include <cstddef>
#include <functional>
#include <limits>
#include <queue>

std::pair<int, int> operator+(const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
    return std::make_pair(p1.first + p2.first, p1.second + p2.second);
}

long shortestPath(int startI, int startJ, 
                  const std::vector<std::vector<bool>>& grid,
                  int destI, int destJ)
{
    if (grid.empty() || grid[0].empty()) {
        return -1L;
    }

    int n = grid.size();
    int m = grid[0].size();
    if (startI >= n || startJ >= m || destI >= n || destJ >= m ||
        startI < 0 || startJ < 0 || destI < 0 || destJ < 0 ||
        grid[startI][startJ] || grid[destI][destJ]) {
        return -1L;
    }

    if (startI == destI && startJ == destJ) {
        return 0L;
    }

    std::function<bool(int, int)> inBounds = [n, m](int i, int j) {
        return i >= 0 && j >= 0 && i < n && j < m;
    };

    std::vector<std::vector<long>> arr(n, std::vector<long>(m, std::numeric_limits<long>::max()));
    std::vector<std::pair<int, int>> directions = {
        {1, 0},
        {0, 1},
        {-1, 0},
        {0, -1}
    };

    using Pos = std::pair<int, int>;
    std::queue<Pos> q;
    arr[startI][startJ] = 0;
    q.push({startI, startJ});
    while (!q.empty()) {
        Pos curr = q.front();
        q.pop();
        int ci = curr.first;
        int cj = curr.second;

        for (const auto& d : directions) {
            Pos next = curr + d;
            int ni = next.first;
            int nj = next.second;
            if (inBounds(ni, nj) && !grid[ni][nj] && arr[ni][nj] == std::numeric_limits<long>::max()) {
                arr[ni][nj] = arr[ci][cj] + 1;
                if (ni == destI && nj == destJ) {
                    return arr[destI][destJ];
                }
                
                q.push(next);
            }
        }
    }

    return -1L;
}
