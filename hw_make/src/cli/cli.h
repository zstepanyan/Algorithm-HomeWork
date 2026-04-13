#ifndef MYMAKE_CLI_CLI_H
#define MYMAKE_CLI_CLI_H

#include "core/config.h"

namespace mymake {

// Parse command-line arguments into a Config.
// Returns the Config on success, or prints usage and exits on error.
Config parse_args(int argc, char* argv[]);

} // namespace mymake

#endif // MYMAKE_CLI_CLI_H
