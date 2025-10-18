#pragma once

#include <pthread.h>

#include "exceptions.h"

namespace thread {
    using threadFunc = typeof(void*(void*))*;
    struct threadInfo;
    using threadHandle = threadInfo*;  
    
    class Thread {
      private:
        threadFunc func;
        threadHandle handle;
      public:
        Thread(threadFunc func);
        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;
        Thread(Thread&& other);
        Thread& operator=(Thread&& other);
        void Run(void* threadData);
        void Join();
        ~Thread() noexcept;
    };
}