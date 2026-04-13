#ifndef MYMAKE_CORE_SOURCE_LOCATION_H
#define MYMAKE_CORE_SOURCE_LOCATION_H

#include <string>

namespace mymake {

struct SourceLocation {
    std::string filename;
    int line = 0;

    SourceLocation() = default;
    SourceLocation(std::string fname, int ln)
        : filename(std::move(fname)), line(ln) {}
};

} // namespace mymake

#endif // MYMAKE_CORE_SOURCE_LOCATION_H
