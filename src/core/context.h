#pragma once

#include <iostream>

struct Context {
    bool running = true;
    std::string test = "testcontext";
};

extern Context* ctx;