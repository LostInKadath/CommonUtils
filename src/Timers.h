#pragma once

#include <string>
#include <chrono>

namespace sra {
namespace time {

class Timer
{
	struct AutoTimeMeasurer
	{
		explicit AutoTimeMeasurer(Timer& parent)
			: m_parent(parent)
			, m_start(std::chrono::steady_clock::now())
		{}

		~AutoTimeMeasurer()
		{
			m_parent.m_duration = std::chrono::steady_clock::now() - m_start;
		}

	private:
		Timer& m_parent;
		std::chrono::steady_clock::time_point m_start{};
	};

public:
	/*Measures execution time. Gets lambda object which captures all needed parameters.
	Usage:
		ResultType Foo(int, bool, Arg const&);

		auto result = timer.Measure([&] { return Foo(1, true, arg); });
	*/
	template <typename Lambda>
	auto Measure(Lambda&& fn)
	{
		AutoTimeMeasurer timer{ *this };
		return std::forward<Lambda>(fn)();
	}

	/*Measures execution time. Gets pointer to a function and its arguments.
	Usage:
		ResultType Foo(int, bool, Arg const&);

		auto result = timer.Measure(Foo, 1, true, arg);
	*/
	template<typename FuncPointer, typename... Args>
	auto Measure(FuncPointer&& fn, Args&&... args)
	{
		AutoTimeMeasurer timer{ *this };
		return std::forward<FuncPointer>(fn)(std::forward<Args>(args)...);
	}

public:
	std::chrono::nanoseconds GetTimeNs() const { return m_duration; }
	std::string GetTime() const;

private:
	std::chrono::nanoseconds m_duration{ 0 };
};

}
}   // sra::time
