#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

#include "os.h"

namespace {
    inline std::string BASEDIRECTORYFORFILES = "../testfiles/";
}

static void signal_handler(int sig) {}

namespace child {
    class Child {
      private:
        int pid;

        std::string filename;
        std::ofstream file;

        char* shm_ptr;
        size_t shm_size = SHMSIZE;
        std::string shm_name;
      public:
        Child(const std::string& shm_name, const std::string& filename);
        void Work();
        ~Child();
    };
}