#pragma once

#include <exception>
#include <string>

namespace exceptions {
    class CreateThreadException: std::exception {
    public:
        explicit CreateThreadException(const std::string& text): error_message_(text) {}
        const char* what() const noexcept override {
            return error_message_.data();
        }
    private:
        std::string error_message_;
    };

    class WaitThreadException: std::exception {
    public:
        explicit WaitThreadException(const std::string& text): error_message_(text) {}
        const char* what() const noexcept override {
            return error_message_.data();
        }
    private:
        std::string error_message_;
    };
}


