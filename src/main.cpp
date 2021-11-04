#include <functional>
#include <iostream>

#include <spdlog/spdlog.h>

int main(int, const char**)
{
    //Use the default logger (stdout, multi-threaded, colored)
    spdlog::info("Hello, {}!", "World");
}
