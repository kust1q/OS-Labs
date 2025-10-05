#include "os.h" 
#include <unistd.h>

namespace os {
    int CreatePipe(int pipefd[2]) {return pipe(pipefd);}

    pid_t Fork() {return fork();};

    int Dup2(int fd1, int fd2) {return dup2(fd1, fd2);}

    int CloseFd(int fd) {return close(fd);}

    int Exec(const char* processPath, const char* processName) {
        execl(processPath, processName, NULL);
        return -1;
    }
    int KillProcess(pid_t pid) {
        return kill(pid, SIGTERM);
    }
    ssize_t WriteStr(int fd, const char* buf, size_t bytes) {
        return write(fd, buf, bytes);
    }  
    pid_t GetPid() {
        return getpid();
    }

    pid_t GetPPid() {
        return getppid();
    }

    bool IsAliveProcess(pid_t pid) {
        return waitpid(pid, nullptr, WNOHANG) == 0;
    }

    void Exit(int status) {
        return _exit(status);
    }

    unsigned int Sleep(unsigned int t) {
        return sleep(t);
    }
}