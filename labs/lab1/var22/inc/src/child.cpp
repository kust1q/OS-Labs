#include "child.h"
#include "os.h"

namespace child {
    Child::Child(): Child::Child("") {
    }

    Child::Child(const std::string& filename): pid(os::GetPid()), filename(BASEDIRECTORYFORFILES + filename) {
        file.open(Child::filename, std::ios::app);
        if (!file.is_open()) {
            std::cout << "Child[" << pid << "]: failed to open " << Child::filename << ". Process is over.\n";
            os::Exit(1);
        }
        std::cout << "Child[" << pid << "]: file " << Child::filename << " opened." << std::endl;
    }

    void Child::Work() {
        if (!file.is_open()) {
            std::cout << "Child[" << pid << "]: the file is not open! Work is impossible." << std::endl;
            os::Exit(1);
        }
        std::string line;
        std::string inv; 
        while (std::getline(std::cin, line)) {
            if (line.empty()) {continue;}
            std::cout << "Child[" << pid << "]: the string \"" << line << "\" was got." << std::endl;
            inv = line;
            std::reverse(inv.begin(), inv.end());
            std::cout << "Child[" << pid << "]: the string \"" << line << "\" was reversed." << std::endl;
            file << inv << std::endl;
            std::cout << "Child[" << pid << "]: the string \"" << inv << "\" was written in file \"" << filename << "\"." << std::endl;
        }
    }

    Child::~Child() {
        if (file.is_open()) {
            file.close();
        }
    }
}