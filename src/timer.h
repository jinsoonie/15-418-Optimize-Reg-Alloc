/***
 *
 * TIMER CLASS, instantiate one of these in main and use to measure execution times
 *
 * ***/

#include <iostream>
#include <chrono>

class Timer {
private:
    // Type aliases for easy access to specific time units and clock type
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;

    std::chrono::time_point<clock_t> m_beg;  // Start point of the timer

public:
    Timer() : m_beg(clock_t::now()) {
        // Constructor initializes the timer
    }

    void reset() {
        // Reset the timer to the current time
        m_beg = clock_t::now();
    }

    double elapsed() const {
        // Calculate and return the elapsed time in seconds
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }
};
