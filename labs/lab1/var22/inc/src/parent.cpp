#include "parent.h"

namespace parent {
    void Parent::CreateChildProcesses(std::string filename1, std::string filename2) {
        if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
            std::cout << "Parent[" << getpid() << "]: Не удалось создать пайпы" << std::endl;
            exit(1);
        }

        child1 = fork();
        if (child1 == 0) {
            close(pipe1[1]);
            dup2(pipe1[0], STDIN_FILENO);
            close(pipe1[0]);
            execl("./child", "child1", NULL);
            std::cout << "Parent[" << getpid() << "]: Не удалось запустить первый дочерний процесс." << std::endl; 
            exit(1);
        }
        std::cout << "Child[" << child1 << "]: процесс создан." << std::endl;
        child2 = fork();
        if (child2 == 0) {
            close(pipe2[1]);
            dup2(pipe2[0], STDIN_FILENO);
            close(pipe2[0]);
            execl("./child", "child2", NULL);
            std::cout << "Parent[" << getpid() << "]: Не удалось запустить второй дочерний процесс." << std::endl; 
            exit(1);
        }
        std::cout << "Child[" << child2 << "]: процесс создан." << std::endl;
        close(pipe1[0]);
        close(pipe2[0]);

        filename1 += "\n";
        filename2 += "\n";
    
        write(pipe1[1], filename1.c_str(), filename1.length());
        write(pipe2[1], filename2.c_str(), filename2.length());
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
                write(pipe1[1], input.c_str(), input.length());
            } else {
                write(pipe2[1], input.c_str(), input.length());
            }
        }
    } 

    void Parent::EndChildren() {
        if (child1 > 0) {
            kill(child1, SIGTERM);
            waitpid(child1, nullptr, 0);
            std::cout << "Child[" << child1 << "]: процесс завершен." << std::endl;
        }
        if (child2 > 0) {
            kill(child2, SIGTERM);
            waitpid(child2, nullptr, 0);
            std::cout << "Child[" << child2 << "]: процесс завершен." << std::endl;
        }
    }

    Parent::~Parent() {
        close(pipe1[0]); 
        close(pipe1[1]);
        close(pipe2[0]); 
        close(pipe2[1]);
    }
}