#ifndef CONTEXT_H
#define CONTEXT_H

#include <iostream>

struct Context {
    bool running = true;
    std::string test = "Hello World!";
};

extern Context* ctx;

#endif // CONTEXT_H