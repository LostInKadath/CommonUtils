#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

template <typename T> std::ostream& operator<<(std::ostream&, const std::vector<T>&);

template std::ostream& operator<<<>(std::ostream&, const std::vector<char>&);
template std::ostream& operator<<<>(std::ostream&, const std::vector<signed char>&);
template std::ostream& operator<<<>(std::ostream&, const std::vector<unsigned char>&);

template std::ostream& operator<<<>(std::ostream&, const std::vector<short>&);
template std::ostream& operator<<<>(std::ostream&, const std::vector<int>&);
template std::ostream& operator<<<>(std::ostream&, const std::vector<long>&);
template std::ostream& operator<<<>(std::ostream&, const std::vector<long long>&);

template std::ostream& operator<<<>(std::ostream&, const std::vector<unsigned short>&);
template std::ostream& operator<<<>(std::ostream&, const std::vector<unsigned int>&);
template std::ostream& operator<<<>(std::ostream&, const std::vector<unsigned long>&);
template std::ostream& operator<<<>(std::ostream&, const std::vector<unsigned long long>&);

template std::ostream& operator<<<>(std::ostream&, const std::vector<float>&);
template std::ostream& operator<<<>(std::ostream&, const std::vector<double>&);
template std::ostream& operator<<<>(std::ostream&, const std::vector<long double>&);

template std::ostream& operator<<<>(std::ostream&, const std::vector<std::string>&);
