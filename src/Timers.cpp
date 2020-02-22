#include "timers.h"
#include <sstream>

std::string sra::time::Timer::GetTime() const
{
	const auto ns = m_duration.count();
	const auto mcs = std::div(ns, 1000ll);
	const auto ms = std::div(mcs.quot, 1000ll);
	const auto s = std::div(ms.quot, 1000ll);

	std::stringstream os;
	os << s.quot << "s " << s.rem << "ms " << ms.rem << "mcs " << mcs.rem << "ns";
	return os.str();
}
