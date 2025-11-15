#include <signal.h>
#include <cstring>

#include "child.h"
#include "os.h"



namespace child {
    Child::Child(const std::string& shm_name, const std::string& filename): pid(os::GetPid()), filename(BASEDIRECTORYFORFILES + filename), shm_name(shm_name) {
        auto shm_handle = os::CreateSharedMemory(shm_name.c_str(), shm_size);
        if (!shm_handle) {
            std::cout << "Child[" << pid << "]: failed to open shared memory. Process is over.\n";
            os::Exit(1);
        }

        shm_ptr = os::MapSharedMemory(shm_handle, shm_size);
        if (!shm_ptr) {
            std::cout << "Child[" << pid << "]: failed to map shared memory. Process is over.\n";
            os::Exit(1);
        }

        file.open(Child::filename, std::ios::app);
        if (!file.is_open()) {
            std::cout << "Child[" << pid << "]: failed to open " << Child::filename << ". Process is over.\n";
            os::Exit(1);
        }
        std::cout << "Child[" << pid << "]: file " << Child::filename << " opened." << std::endl;
        os::Signal(os::SIGNAL_DATA_READY, signal_handler);
    }

    void Child::Work() {
        if (!file.is_open()) {
            std::cout << "Child[" << pid << "]: the file is not open! Work is impossible." << std::endl;
            os::Exit(1);
        }

        std::string line;
        std::string inv;

        while (true) {

            os::Pause();

            line = shm_ptr;

            if (line.empty()) continue;

            std::cout << "Child[" << pid << "]: the string " << line << " was got." << std::endl;

            inv = line;
            std::reverse(inv.begin(), inv.end());
            std::cout << "Child[" << pid << "]: the string " << line << " was reversed." << std::endl;

            file << inv << std::endl;
            std::cout << "Child[" << pid << "]: the string " << inv << " was written in file " << filename << "." << std::endl;
        }
    }

    Child::~Child() {
        if (file.is_open()) {
            file.close();
        }
    }
}