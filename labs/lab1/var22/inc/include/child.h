#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

namespace child {
    class Child {
      private:
        std::string filename;
        std::ofstream file;
        pid_t pid;
      public:
        Child();
        Child(const std::string& filename);
        void Work();
        ~Child();
    };
}