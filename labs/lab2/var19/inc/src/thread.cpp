#include "thread.h"

namespace thread {
    struct threadInfo {
        pthread_t thread;
    };

    Thread::Thread(threadFunc func): func(func) {
        handle = new threadInfo();
    }

    Thread::Thread(Thread&& other): func(other.func), handle(other.handle) {
        other.func = nullptr;
        other.handle = nullptr;
    }

    Thread& Thread::operator=(Thread&& other) {
        Thread temp = std::move(other);
        std::swap(func, temp.func);
        std::swap(handle, temp.handle);
        return *this;
    }

    void Thread::Run(void* data) {
        int res = pthread_create(&(handle->thread), NULL, func, data);
        if (res != 0) {
            throw exceptions::CreateThreadException("failed to create thread");
        }
    }

    void Thread::Join() {
        int res = pthread_join(handle->thread, NULL);
        if (res != 0) {
            throw exceptions::WaitThreadException("Failed to wait thred");
        }
    }
    
    Thread::~Thread() {
        delete handle;
    }
}