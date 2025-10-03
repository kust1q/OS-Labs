#include "parent.h"

int main() {
    std::string filename1;
    std::string filename2;
    std::cout << "Parent[" << getpid() << "]: Введите имя файла для child1" << std::endl;
    std::getline(std::cin, filename1);
    std::cout << "Parent: Введите имя файла для child2" << std::endl;
    std::getline(std::cin, filename2);
    parent::Parent p;
    p.CreateChildProcesses(filename1, filename2);
    p.Work();
    p.EndChildren();
    return 0;
}