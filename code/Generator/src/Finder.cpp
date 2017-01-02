#include "Finder.hpp"

namespace flex {
std::map<std::string, unsigned long>& find(std::istream& in, std::ostream& out);
}

std::map<std::string, unsigned long>& Finder::run(std::istream &in, std::ostream &out) {
    return flex::find(in, out);
}
