#include <chrono>

#ifdef __APPLE__
std::chrono::time_point<
    std::chrono::steady_clock> GetTime() {
		return std::chrono::steady_clock::now();
}
#else
std::chrono::time_point<
    std::chrono::monotonic_clock> GetTime() {
		return std::chrono::monotonic_clock::now();
}
#endif