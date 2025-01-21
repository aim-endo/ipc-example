#include "shared_memory.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <stdexcept>

void SharedMemory::send(const std::string& path)
{
    const char* shnName = "/shared_memory";
    const size_t bufferSize = 1024;

    int shmFd = shm_open(shnName, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shmFd == -1) {
        std::cerr << "shm_open failed: " << strerror(errno) << std::endl;
        return;
    }

    if (ftruncate(shmFd, bufferSize) == -1) {
        std::cerr << "ftruncate failed: " << strerror(errno) << std::endl;
        return;
    }

    void* buffer = mmap(0, bufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    if (buffer == MAP_FAILED) {
        std::cerr << "mmap failed: " << strerror(errno) << std::endl;
        return;
    }

    std::string message = "sending file: " + path;
    strncpy(static_cast<char*>(buffer), message.c_str(), bufferSize);

    munmap(buffer, bufferSize);
    close(shmFd);
}

void SharedMemory::receive()
{
    const char* shnName = "/shared_memory";
    const size_t bufferSize = 1024;

    int shmFd = shm_open(shnName, O_RDONLY, 0);
    if (shmFd == -1) {
        std::cerr << "shm_open failed: " << strerror(errno) << std::endl;
        return;
    }

    void* buffer = mmap(0, bufferSize, PROT_READ, MAP_SHARED, shmFd, 0);
    if (buffer == MAP_FAILED) {
        std::cerr << "mmap failed: " << strerror(errno) << std::endl;
        return;
    }

    std::cout << "Received: " << static_cast<char*>(buffer) << std::endl;

    munmap(buffer, bufferSize);
    close(shmFd);

    shm_unlink(shnName);
}
