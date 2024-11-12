#include <iostream>
#include "core/context.h"


int main() {
    std::cout << ctx -> test << std::endl;
    #ifdef _DEBUG
        std::cout << "This is a debug build" << std::endl;
    #else
        std::cout << "This is a release build" << std::endl;
    #endif
    return 0;
}
