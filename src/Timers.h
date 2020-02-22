#pragma once

#include <string>
#include <sstream>
#include <chrono>

namespace sra {
namespace time {

class Timer
{
public:
	template<typename Fn>
	auto Measure(Fn&& fn)
	{
		const auto start = std::chrono::steady_clock::now();
		const auto result = fn();
		const auto stop = std::chrono::steady_clock::now();

		m_duration = stop - start;
		return result;
	}

	std::chrono::nanoseconds GetTimeNs() const { return m_duration; }
	std::string GetTime() const;

private:
	std::chrono::nanoseconds m_duration{ 0 };
};

}
}   // sra::time
