#include "../include/child.h"


namespace child {
    Child::Child(): Child::Child("") {
    }

    Child::Child(const std::string& filename): pid(getpid()), filename(filename) {
        file.open(filename, std::ios::app);
        if (!file.is_open()) {
            std::cout << "Child[" << pid << "]: не удалось открыть файл " << filename << ". Процесс завершен.\n";
            return;
        }
        std::cout << "Child[" << pid << "]: файл " << filename << " открыт." << std::endl;
    }

    void Child::Work() {
        if (!file.is_open()) {
            std::cout << "Child[" << pid << "]: файл не открыт! Работа невозможна." << std::endl;
            return;
        }
        std::string line;
        std::string inv;
        while (std::getline(std::cin, line)) {
            if (line.empty()) {continue;}
            std::cout << "Child[" << pid << "]: строка " << line << " получена." << std::endl;
            inv = line;
            std::reverse(inv.begin(), inv.end());
            std::cout << "Child[" << pid << "]: строка " << line << " инвертирована." << std::endl;
            file << inv << std::endl;
            file.flush(); 
            std::cout << "Child[" << pid << "]: строка " << inv << " записана в файл " << filename << "." << std::endl;
        }

        std::cout << "Child[" << pid << "]: процесс завершен." << std::endl;
    }

    Child::~Child() {
        if (file.is_open()) {
            file.close();
        }
    }
}