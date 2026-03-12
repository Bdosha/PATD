#ifndef STREAM_EXCEPTIONS_H
#define STREAM_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class EndOfStream : public std::runtime_error {
public:
    explicit EndOfStream(const std::string &message = "End of stream reached")
        : std::runtime_error(message) {}

    explicit EndOfStream(const char *message)
        : std::runtime_error(message) {}
};

#endif //STREAM_EXCEPTIONS_H

