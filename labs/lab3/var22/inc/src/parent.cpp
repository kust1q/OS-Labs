#include "exceptions.h"
#include "parent.h"
#include "os.h"

namespace parent {
    Parent::Parent() {
        shm_handle1 = os::CreateSharedMemory(shm_name1.c_str(), shm_size);
        shm_ptr1 = os::MapSharedMemory(shm_handle1, shm_size);

        shm_handle2 = os::CreateSharedMemory(shm_name2.c_str(), shm_size);
        shm_ptr2 = os::MapSharedMemory(shm_handle2, shm_size);
    }

    void Parent::CreateChildProcesses(std::string filename1, std::string filename2) {
        child1 = os::CreateProcess(ProcessParams{"./child", "child1", shm_name1.c_str(), filename1.c_str()});
        std::cout << "Child1[" << os::GetChildPid(child1) << "]: process was created." << std::endl;

        child2 = os::CreateProcess(ProcessParams{"./child", "child2", shm_name2.c_str(), filename2.c_str()});
        std::cout << "Child2[" << os::GetChildPid(child2) << "]: process was created." << std::endl;

        os::Sleep(1);

        if (!os::IsAliveProcess(child1) || !os::IsAliveProcess(child2)) {
            throw exceptions::ChildProcessEndException("Child process already terminated!");
        }
    }

    void Parent::Work() {
        std::string input;
        while (std::getline(std::cin, input)) {
            if (input == "exit" || input == "quit") {
                std::cout << "Parent: termination command was received." << std::endl;
                break;
            }
            if (!os::IsAliveProcess(child1) || !os::IsAliveProcess(child2)) {
                throw exceptions::ChildProcessEndException("Child process already terminated!");
            }
            if (input.empty()) continue;
            if (rd() % 100 < 80) {
                strncpy(shm_ptr1, input.c_str(), shm_size - 1);
                os::SendSignal(child1, os::SIGNAL_DATA_READY);
            } else {
                strncpy(shm_ptr2, input.c_str(), shm_size - 1);
                os::SendSignal(child2, os::SIGNAL_DATA_READY);
            }
        }
    }

    void Parent::EndChildren() {
        os::TerminateProcess(child1);
        std::cout << "Child1: process was terminated." << std::endl;
        os::TerminateProcess(child2);
        std::cout << "Child2: process was terminated." << std::endl;
    }

    Parent::~Parent() {
        os::UnmapSharedMemory(shm_ptr1, shm_size);
        os::CloseSharedMemory(shm_handle1);

        os::UnmapSharedMemory(shm_ptr2, shm_size);
        os::CloseSharedMemory(shm_handle2);
    }
}