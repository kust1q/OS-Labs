#pragma once

#include <exception>
#include <string>

namespace exceptions {
    class ChildProcessEndException: std::exception {
    public:
        explicit ChildProcessEndException(const std::string& text): error_message_(text) {}
        const char* what() const noexcept override {
            return error_message_.data();
        }
    private:
        std::string error_message_;
    };
}