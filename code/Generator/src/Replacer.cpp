#include "Generator/inc/Replacer.hpp"

namespace flex {
void replace(std::istream& in, std::ostream& out, std::map<std::string, unsigned long>& places);
}

void Replacer::run(std::istream &in, std::ostream &out, std::map<std::string, unsigned long> &places) {
    flex::replace(in, out, places);
}
