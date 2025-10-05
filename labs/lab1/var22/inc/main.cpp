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
    } catch (const exceptions::CreatePipeException&) {
        std::cout << "Parent[" << os::GetPid() << "]: Failed to create pipe. Ending programm." << std::endl;
        return 1;
    } catch (const exceptions::ChildProcessEndException&) {
        std::cout << "Parent[" << os::GetPid() << "]: Child process is ended with error. Ending programm." << std::endl;
        return 1;
    }
    return 0;
}