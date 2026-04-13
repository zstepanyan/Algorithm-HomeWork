#ifndef MYMAKE_CORE_CONFIG_H
#define MYMAKE_CORE_CONFIG_H

#include <string>
#include <unordered_map>
#include <vector>

namespace mymake {

struct Config {
    std::string makefile_path;
    std::vector<std::string> goals;
    std::unordered_map<std::string, std::string> overrides;
    int jobs = 1;
    bool dry_run = false;
    bool always_make = false;
    bool keep_going = false;
    bool silent = false;
    std::string directory;
    bool debug = false;
};

} // namespace mymake

#endif // MYMAKE_CORE_CONFIG_H
