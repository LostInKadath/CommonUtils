#pragma once

#include <string>
#include <chrono>

namespace sra {
namespace time {

class Timer
{
public:

	/*Measures execution time. Gets lambda object which captures all needed parameters.
	Usage:
		ResultType Foo(int, bool, Arg const&);

		auto result = timer.Measure([&] { return Foo(1, true, arg); });
	*/
	template <typename Lambda>
	auto Measure(Lambda&& fn)
	{
		const auto start = std::chrono::steady_clock::now();
		const auto result = fn();
		const auto stop = std::chrono::steady_clock::now();

		m_duration = stop - start;
		return result;
	}

	/*Measures execution time. Gets pointer to a function and its arguments.
	Usage:
		ResultType Foo(int, bool, Arg const&);

		auto result = timer.Measure(Foo, 1, true, arg);
	*/
	template<typename FuncPointer, typename... Args>
	auto Measure(FuncPointer fn, Args&&... args)
	{
		const auto start = std::chrono::steady_clock::now();
		const auto result = fn(std::forward<Args>(args)...);
		const auto stop = std::chrono::steady_clock::now();

		m_duration = stop - start;
		return result;
	}

public:
	std::chrono::nanoseconds GetTimeNs() const { return m_duration; }
	std::string GetTime() const;

private:
	std::chrono::nanoseconds m_duration{ 0 };
};

}
}   // sra::time
