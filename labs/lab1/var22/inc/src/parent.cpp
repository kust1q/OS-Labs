#include "parent.h"
#include "os.h"

namespace parent {
    void Parent::CreateChildProcesses(std::string filename1, std::string filename2) {
        if (os::CreatePipe(pipe1) == -1 || os::CreatePipe(pipe2) == -1) {
            std::cout << "Parent[" << getpid() << "]: Не удалось создать пайпы" << std::endl;
            exit(1);
        }

        child1 = os::Fork();
        if (child1 == 0) {
            os::CloseFd(pipe1[1]);
            os::Dup2(pipe1[0], STDIN_FILENO);
            os::CloseFd(pipe1[0]);
            os::Exec("./child", "child1");
            std::cout << "Parent[" << getpid() << "]: Не удалось запустить первый дочерний процесс." << std::endl; 
            exit(1);
        }
        std::cout << "Child[" << child1 << "]: процесс создан." << std::endl;
        child2 = os::Fork();
        if (child2 == 0) {
            os::CloseFd(pipe2[1]);
            os::Dup2(pipe2[0], STDIN_FILENO);
            os::CloseFd(pipe2[0]);
            os::Exec("./child", "child2");
            std::cout << "Parent[" << getpid() << "]: Не удалось запустить второй дочерний процесс." << std::endl; 
            exit(1);
        }
        std::cout << "Child[" << child2 << "]: процесс создан." << std::endl;
        os::CloseFd(pipe1[0]);
        os::CloseFd(pipe2[0]);

        filename1 += "\n";
        filename2 += "\n";
    
        os::WriteStr(pipe1[1], filename1.c_str(), filename1.length());
        os::WriteStr(pipe2[1], filename2.c_str(), filename2.length());
    }

    void Parent::Work() {
        std::string input;
        while (std::getline(std::cin, input)) {
            if (input.empty()) {continue;}
            if (input == "exit" || input == "quit") {
                std::cout << "Parent[" << getpid() << "]: получена команда выхода из программы." << std::endl;
                break;
            }
            input += "\n";
            if (rd() % 100 < 80) {
                os::WriteStr(pipe1[1], input.c_str(), input.length());
            } else {
                os::WriteStr(pipe2[1], input.c_str(), input.length());
            }
        }
    } 

    void Parent::EndChildren() {
        if (child1 > 0) {
            os::KillProcess(child1);
            os::WaitProcess(child1);
            std::cout << "Child[" << child1 << "]: процесс завершен." << std::endl;
        }
        if (child2 > 0) {
            os::KillProcess(child2);
            os::WaitProcess(child2);
            std::cout << "Child[" << child2 << "]: процесс завершен." << std::endl;
        }
    }

    Parent::~Parent() {
        os::CloseFd(pipe1[0]); 
        os::CloseFd(pipe1[1]);
        os::CloseFd(pipe2[0]); 
        os::CloseFd(pipe2[1]);
    }
}