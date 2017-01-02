#pragma once

#include <iosfwd>
#include <map>

class Replacer {
public:
    void run(std::istream &in, std::ostream &out, std::map<std::string, unsigned long> &places);
};

