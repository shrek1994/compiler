#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>

enum Instructions { GET, PUT, LOAD, STORE, COPY, ADD, SUB, SHR, SHL, INC, DEC, ZERO, JUMP, JZERO, JODD, HALT, ERROR };

std::ostream& operator<< (std::ostream& out, Instructions instructions);

class Interpreter
{
public:
    int run(std::istream& inFile,
            std::istream& cin = std::cin,
            std::ostream& cout = std::cout,
            std::ostream& info = std::cout);
};

#endif // INTERPRETER_H
