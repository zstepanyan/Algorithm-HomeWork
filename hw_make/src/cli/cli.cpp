#include "cli/cli.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>

namespace mymake {

static void print_usage() {
    std::cerr << "Usage: my_make [options] [target] ...\n"
              << "Options:\n"
              << "  -f FILE    Read FILE as a makefile\n"
              << "  -j N       Allow N jobs at once\n"
              << "  -n         Dry run (print commands without executing)\n"
              << "  -B         Unconditionally make all targets\n"
              << "  -k         Keep going when some targets can't be made\n"
              << "  -s         Silent mode (don't print commands)\n"
              << "  -C DIR     Change to DIR before doing anything\n"
              << "  --debug    Print debug information\n"
              << "  -h         Print this help message\n";
}

Config parse_args(int argc, char* argv[]) {
    Config config;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-f" && i + 1 < argc) {
            config.makefile_path = argv[++i];
        } 
        else if (arg == "-j" && i + 1 < argc) {
            config.jobs = std::atoi(argv[++i]);
            if (config.jobs < 1) config.jobs = 1;
        } 
        else if (arg == "-n") {
            config.dry_run = true;
        } 
        else if (arg == "-B") {
            config.always_make = true;
        } 
        else if (arg == "-k") {
            config.keep_going = true;
        } 
        else if (arg == "-s") {
            config.silent = true;
        } 
        else if (arg == "-C" && i + 1 < argc) {
            config.directory = argv[++i];
        } 
        else if (arg == "--debug") {
            config.debug = true;
        } 
        else if (arg == "-h" || arg == "--help") {
            print_usage();
            std::exit(0);
        } 
        else if (arg.find('=') != std::string::npos) {
            // VAR=value override
            auto eq = arg.find('=');
            config.overrides[arg.substr(0, eq)] = arg.substr(eq + 1);
        } 
        else if (arg[0] == '-') {
            std::cerr << "my_make: Unknown option: " << arg << std::endl;
            print_usage();
            std::exit(1);
        } 
        else {
            config.goals.push_back(arg);
        }
    }

    // Auto-detect makefile if not specified
    if (config.makefile_path.empty()) {
        if (std::filesystem::exists("GNUmakefile")) {
            config.makefile_path = "GNUmakefile";
        } 
        else if (std::filesystem::exists("makefile")) {
            config.makefile_path = "makefile";
        } 
        else if (std::filesystem::exists("Makefile")) {
            config.makefile_path = "Makefile";
        } 
        else {
            std::cerr << "my_make: *** No targets specified and no makefile found.  Stop."
                      << std::endl;
            std::exit(2);
        }
    }

    return config;
}

} // namespace mymake
