#pragma once

#include <cstddef>
#include <unistd.h>
#include <sys/wait.h>

namespace os {
    int CreatePipe(int pipefd[2]);
    pid_t Fork();
    int Dup2(int fd1, int fd2);
    int CloseFd(int fd);
    int Exec(const char* processPath, const char* processName);
    pid_t WaitProcess(pid_t pid);
    int KillProcess(pid_t pid);
    ssize_t WriteStr(int fd, const char* buf, size_t bytes);
}