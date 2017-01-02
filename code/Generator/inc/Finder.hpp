#pragma once

#include <iostream>
#include <map>
#include <string>

class Finder {
public:
    std::map<std::string, unsigned long>& run(std::istream& in, std::ostream& out);
};

