#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include <string>

template<class T>
using Deserializer = T (*)(const std::string &str);

#endif

