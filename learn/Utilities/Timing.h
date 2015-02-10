#pragma once

#include <chrono>

#ifdef __APPLE__
extern std::chrono::time_point<std::chrono::steady_clock> GetTime();
#else
extern std::chrono::time_point<std::chrono::monotonic_clock> GetTime();
#endif