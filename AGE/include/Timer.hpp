#include <chrono>

namespace age {

class Timer {
    std::chrono::steady_clock::time_point m_time;
public:
    Timer() {
        m_time = std::chrono::high_resolution_clock::now();
    }

    float time() {
        auto current = std::chrono::high_resolution_clock::now();

        return std::chrono::duration<float, std::chrono::seconds::period>(current - m_time).count();
    }

    float reset() {
        auto current = std::chrono::high_resolution_clock::now();
        float elapsed = std::chrono::duration<float, std::chrono::seconds::period>(current - m_time).count();
        m_time = current;

        return elapsed;
    }
};

} // namespace age
