#pragma once

#include <cstddef>
#include <iostream>
#include <random>
#include <unistd.h>
#include <sys/wait.h>

namespace parent {
    class Parent {
      private:
        std::random_device rd;
        int pipe1[2];
        int pipe2[2];
        pid_t child1;
        pid_t child2;
      public:
        Parent() = default;
        void CreateChildProcesses(std::string filename1, std::string filename2);
        void Work();
        void EndChildren();
        ~Parent();
    };
}