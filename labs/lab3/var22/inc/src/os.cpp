#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "os.h"

struct ProcessHandleData {
    pid_t pid;
};

struct SharedMemHandleData {
    int fd;
    size_t size;
};

namespace os {
    ProcessHandle CreateProcess(const ProcessParams& params) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Error: failed to fork");
            throw std::runtime_error("Fork failed");
        }
        if (pid == 0) {
            execl(params.app, params.arg1, params.arg2, params.arg3, NULL);
            perror("Error: failed to execl");
            _exit(1);
        }
        auto* data = new ProcessHandleData{pid};
        return reinterpret_cast<ProcessHandle>(data);
    }

    int GetChildPid(ProcessHandle handle) {
        ProcessHandleData* data = reinterpret_cast<ProcessHandleData*>(handle);
        return data->pid;
    }

    void TerminateProcess(ProcessHandle handle) {
        ProcessHandleData* data = reinterpret_cast<ProcessHandleData*>(handle);
        pid_t pid = data->pid;
        if (kill(pid, SIGTERM) == -1) {
            perror("Warning: failed to terminate process");
        }
        int status;
        waitpid(pid, &status, 0);
        delete data; 
    }

    void SendSignal(ProcessHandle handle, int signal) {
        ProcessHandleData* data = reinterpret_cast<ProcessHandleData*>(handle);
        pid_t pid = data->pid;
        if (kill(pid, signal) == -1) {
            perror("Warning: failed to send signal");
        }
    }

    bool IsAliveProcess(ProcessHandle handle) {
        ProcessHandleData* data = reinterpret_cast<ProcessHandleData*>(handle);
        pid_t pid = data->pid;
        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);
        if (result == -1) {
            perror("Error: failed to waitpid");
            return false;
        }
        return (result == 0);
    }

    void Sleep(int seconds) {
        sleep(seconds);
    }

    SharedMemHandle CreateSharedMemory(const char* name, size_t size) {
        int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        if (fd == -1) {
            perror("Error: failed to shm_open");
            throw std::runtime_error("Shm_open failed");
        }
        if (ftruncate(fd, size) == -1) {
            perror("Error: failed to ftruncate");
            close(fd);
            shm_unlink(name);
            throw std::runtime_error("Ftruncate failed");
        }
        auto* data = new SharedMemHandleData{fd, size};
        return reinterpret_cast<SharedMemHandle>(data);
    }

    char* MapSharedMemory(SharedMemHandle handle, size_t size) {
        SharedMemHandleData* data = reinterpret_cast<SharedMemHandleData*>(handle);
        void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, data->fd, 0);
        if (ptr == MAP_FAILED) {
            throw std::runtime_error("Error: failed to mmap");
        }
        return reinterpret_cast<char*>(ptr);
    }

    void UnmapSharedMemory(char* ptr, size_t size) {
        if (munmap(ptr, size) == -1) {
            perror("Warning: failed to munmap");
        }
    }

    void CloseSharedMemory(SharedMemHandle handle) {
        SharedMemHandleData* data = reinterpret_cast<SharedMemHandleData*>(handle);
        if (close(data->fd) == -1) {
            perror("Warning: failed to close fd");
        }
        delete data;
    }

    void Signal(int sig, SignalHandle handle) {
        if (signal(sig, handle) == SIG_ERR) {
            perror("Warning: signal failed");
        }
    }

    int Pause() {
        return pause();
    }

    int GetPid() {
        return getpid();
    }

    void Exit(int status) {
        _exit(status);
    }
}