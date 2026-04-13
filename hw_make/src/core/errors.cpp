#include "core/errors.h"

namespace mymake {

MakeError::MakeError(const std::string& message, SourceLocation loc)
    : std::runtime_error(message), loc_(std::move(loc)) {}

CommandError::CommandError(const std::string& msg, int exit_code,
                           SourceLocation loc)
    : MakeError(msg, std::move(loc)), exit_code_(exit_code) {}

std::string format_error(const MakeError& err) {
    std::string result;
    const auto& loc = err.location();
    if (!loc.filename.empty()) {
        result += loc.filename;
        if (loc.line > 0) {
            result += ":" + std::to_string(loc.line);
        }
 
        result += ": ";
    }
    
    result += "*** ";
    result += err.what();
    result += ".  Stop.";
 
    return result;
}

} // namespace mymake
