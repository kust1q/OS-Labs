#include "include/child.h"

int main() {
    std::string filename;
    std::getline(std::cin, filename);
    child::Child child(filename);
    child.Work();
    return 0;
}