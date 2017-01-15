
#include "debug.hpp"

namespace Logger {
bool enableDebug = true;
std::ostream& out = std::cerr;

void enable() {
    enableDebug = true;
}

}

