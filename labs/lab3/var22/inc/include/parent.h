#pragma once

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <random>


#include "exceptions.h"
#include "os.h"

namespace parent {
    class Parent {
      private:
        std::random_device rd;
        ProcessHandle child1;
        ProcessHandle child2;
        SharedMemHandle shm_handle1;
        SharedMemHandle shm_handle2;
        char* shm_ptr1;
        char* shm_ptr2;
        std::string shm_name1 = "/shm_child1";
        std::string shm_name2 = "/shm_child2";
        const size_t shm_size = SHMSIZE;

      public:
        Parent();
        void CreateChildProcesses(std::string filename1, std::string filename2);
        void Work();
        void EndChildren();
        ~Parent();
    };
}