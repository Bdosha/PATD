#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>

template<class T>
using Serializer = std::string (*)(const T &item);

#endif //SERIALIZER_H

