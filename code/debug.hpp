#pragma once
#include <iostream>
#include <cstring>
#include <cstdlib>

#define __FILENAME__ (strrchr(__FILE__, '/') ? std::strrchr(__FILE__, '/') + 1 : __FILE__)
#define DEBUG if (Logger::enableDebug) Logger::out << __FILENAME__ << ":" << __LINE__ << ": "

#define ASSERT(expr) \
    if (expr) \
    { } \
    else \
    { \
        Logger::out << __FILENAME__ << ":" << __LINE__ << ": " << "ERROR: " << #expr << "\n"; \
        std::exit(EXIT_FAILURE); \
    }

namespace Logger {
extern bool enableDebug;
extern std::ostream& out;
}
