#include "child.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Error: invalid agrs" << std::endl;
        return 1;
    }

    std::string shm_name = argv[1];
    std::string filename = argv[2];
    child::Child child(shm_name, filename);
    child.Work();
    return 0;
}