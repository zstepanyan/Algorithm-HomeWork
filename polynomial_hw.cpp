#include <list>
#include <vector>
#include <utility>
#include <iostream>

template <typename DegreeType, typename CoeffType>
std::list<std::pair<DegreeType, CoeffType>> polynom_sorter(const std::vector<std::pair<DegreeType, CoeffType>>& input) {
    std::list<std::pair<DegreeType, CoeffType>> list;
    for (const auto& pair : input) {
        DegreeType degree = pair.first;
        CoeffType value = pair.second;

        if (list.empty()) {
            list.emplace_back(degree, value);
        } 
        else {
            bool inserted = false;
            if (list.front().first < degree) {
                list.emplace_front(degree, value);
                inserted = true;
            }
            else if (list.back().first > degree) {
                list.emplace_back(degree, value);
                inserted = true;
            }

            if (!inserted) {
                for (auto it = list.begin(); it != list.end(); ++it) {
                    if (it->first == degree) {
                        it->second += value;
                        inserted = true;
                        break;
                    }
                    if (it->first < degree) {
                        list.insert(it, {degree, value});
                        inserted = true;
                        break;
                    }
                }
            }
        }
    }

    return list;
}
