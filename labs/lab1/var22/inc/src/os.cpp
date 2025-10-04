#include "os.h" 

namespace os {
    int CreatePipe(int pipefd[2]) {return pipe(pipefd);}

    pid_t Fork() {return fork();};

    int Dup2(int fd1, int fd2) {return dup2(fd1, fd2);}

    int CloseFd(int fd) {return close(fd);}

    int Exec(const char* processPath, const char* processName) {
        return execl(processPath, processName, NULL);
    }
    pid_t WaitProcess(pid_t pid) {
        return waitpid(pid, nullptr, 0);
    }
    int KillProcess(pid_t pid) {
        return kill(pid, SIGTERM);
    }
    ssize_t WriteStr(int fd, const char* buf, size_t bytes) {
        return write(fd, buf, bytes);
    }
}