//
// Created by qinzhou on 23-5-9.
//

#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

namespace Canon::Timer {
using namespace std;
using namespace std::chrono;
class Timer {
public:
	Timer() : m_begin(high_resolution_clock::now()) {}
	void reset() {
		m_begin = high_resolution_clock::now();
	}

	template<typename Duration = milliseconds>
		int64_t elapsed() const {
		return duration_cast<Duration>(high_resolution_clock::now() - m_begin).count();
	}

	int64_t elapsed_micro() const {
		return elapsed<milliseconds>();
	}

	int64_t elapsed_nano() {
		return elapsed<nanoseconds>();
	}

	int64_t elapsed_second() const {
		return elapsed<seconds>();
	}

	int64_t elapsed_minutes() const {
		return elapsed<minutes>();
	}

	int64_t elapsed_hours() const {
		return elapsed<hours>();
	}

private:
	time_point<high_resolution_clock> m_begin;

};

}

#endif //TIMER_HPP
