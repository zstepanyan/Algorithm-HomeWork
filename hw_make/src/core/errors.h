#ifndef MYMAKE_CORE_ERRORS_H
#define MYMAKE_CORE_ERRORS_H

#include "core/source_location.h"
#include <stdexcept>
#include <string>

namespace mymake {

class MakeError : public std::runtime_error {
public:
    explicit MakeError(const std::string& message,
                       SourceLocation loc = {});
    const SourceLocation& location() const { return loc_; }

private:
    SourceLocation loc_;
};

class ParseError : public MakeError {
    using MakeError::MakeError;
};

class EvalError : public MakeError {
    using MakeError::MakeError;
};

class CycleError : public MakeError {
    using MakeError::MakeError;
};

class CommandError : public MakeError {
public:
    CommandError(const std::string& msg, int exit_code,
                 SourceLocation loc = {});
    int exit_code() const { return exit_code_; }

private:
    int exit_code_;
};

// Format an error message in GNU make style:
//   filename:line: *** message.  Stop.
std::string format_error(const MakeError& err);

} // namespace mymake

#endif // MYMAKE_CORE_ERRORS_H
