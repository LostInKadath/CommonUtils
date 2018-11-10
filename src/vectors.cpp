#include "vectors.h"

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
	os << "[";
	if (!v.empty())
	{
		for (auto i = 0u; i < v.size() - 1; ++i)
			os << v[i] << ", ";
		os << v.back();
	}
	return os << "]";
}
