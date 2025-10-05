#include "parent.h"

int main() {
    std::string filename1;
    std::string filename2;
    std::cout << "Parent[" << os::GetPid() << "]: Введите имя файла для child1" << std::endl;
    std::getline(std::cin, filename1);
    std::cout << "Parent: Введите имя файла для child2" << std::endl;
    std::getline(std::cin, filename2);
    parent::Parent p;
    try {
        p.CreateChildProcesses(filename1, filename2);
        p.Work();
        p.EndChildren();
    } catch (const exceptions::CreatePipeException&) {
        std::cout << "Parent[" << os::GetPid() << "]: Не удалось создать пайпы. Завершение программы." << std::endl;
        return 1;
    } catch (const exceptions::ChildProcessEndException&) {
        std::cout << "Parent[" << os::GetPid() << "]: Дочерний процесс завершился с ошибкой. Завершение программы." << std::endl;
        return 1;
    }
    return 0;
}