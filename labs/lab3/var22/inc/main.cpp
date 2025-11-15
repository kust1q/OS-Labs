#include "parent.h"

int main() {
    std::string filename1;
    std::string filename2;
    std::cout << "Parent[" << os::GetPid() << "]: Enter filename for child1" << std::endl;
    std::getline(std::cin, filename1);
    std::cout << "Parent[" << os::GetPid() << "]: Enter filename for child2" << std::endl;
    std::getline(std::cin, filename2);
    parent::Parent p;
    try {
        p.CreateChildProcesses(filename1, filename2);
        p.Work();
        p.EndChildren();
    } catch (const exceptions::ChildProcessEndException&) {
        std::cerr << "Parent[" << os::GetPid() << "]: Child process is ended with error. Ending programm." << std::endl;
        return 1;
    } catch (const std::runtime_error&) {
        std::cerr << "Parent[" << os::GetPid() << "]: System calls error. Ending programm." << std::endl;
        return 1;
    }
    return 0;
}