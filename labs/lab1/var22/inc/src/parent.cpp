#include "parent.h"
#include "os.h"

namespace parent {
    void Parent::CreateChildProcesses(std::string filename1, std::string filename2) {
        if (os::CreatePipe(pipe1) == -1 || os::CreatePipe(pipe2) == -1) {
            throw exceptions::CreatePipeException("Failed to create pipe!");
        }

        child1 = os::Fork();
        if (child1 == 0) {
            os::CloseFd(pipe1[1]);
            os::Dup2(pipe1[0], STDIN_FILENO);
            os::CloseFd(pipe1[0]);
            os::Exec("./child", "child1");
            os::Exit(1);
        }
        std::cout << "Child1[" << child1 << "]: process was created." << std::endl;

        child2 = os::Fork();
        if (child2 == 0) {
            os::CloseFd(pipe2[1]);
            os::Dup2(pipe2[0], STDIN_FILENO);
            os::CloseFd(pipe2[0]);
            os::Exec("./child", "child2");
            os::Exit(1);
        }
        std::cout << "Child2[" << child2 << "]: process was created." << std::endl;

        os::Sleep(1);

        os::CloseFd(pipe1[0]);
        os::CloseFd(pipe2[0]);

        filename1 += "\n";
        filename2 += "\n";

        os::WriteStr(pipe1[1], filename1.c_str(), filename1.length());
        os::WriteStr(pipe2[1], filename2.c_str(), filename2.length());

        if (!os::IsAliveProcess(child1) || !os::IsAliveProcess(child2)) {
            Parent::~Parent();
            throw exceptions::ChildProcessEndException("Child process already terminated!");
        }
    }

    void Parent::Work() {
        std::string input;
        while (std::getline(std::cin, input)) {
            if (input == "exit" || input == "quit") {
                std::cout << "Parent[" << os::GetPid() << "]: termination command was received." << std::endl;
                break;
            }
            if (!os::IsAliveProcess(child1) || !os::IsAliveProcess(child2)) {
                Parent::~Parent();
                throw exceptions::ChildProcessEndException("Child process already terminated!");
            }
            if (input.empty()) {continue;}
            input += "\n";
            if (rd() % 100 < 80) {
                os::WriteStr(pipe1[1], input.c_str(), input.length());
            } else {
                os::WriteStr(pipe2[1], input.c_str(), input.length());
            }
        }
    } 

    void Parent::EndChildren() {
        os::KillProcess(child1);
        std::cout << "Child[" << child1 << "]: process was terminated." << std::endl;
        os::KillProcess(child2);
        std::cout << "Child[" << child2 << "]: process was terminated." << std::endl;
    }

    Parent::~Parent() {
        os::CloseFd(pipe1[0]); 
        os::CloseFd(pipe1[1]);
        os::CloseFd(pipe2[0]); 
        os::CloseFd(pipe2[1]);
    }
}