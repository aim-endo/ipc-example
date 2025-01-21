#include "ipc_base.h"
#include "shared_memory.h"
#include "grpc.h"
#include "ucx.h"
#include <iostream>
#include <memory>

int main(const int argc, const char* argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <ipc> <path>" << std::endl;
        return 1;
    }

    const std::string method = argv[1];
    const std::string path = argv[2];

    std::unique_ptr<IPCBase> ipc;

    if (method == "shared_memory") {
        ipc = std::make_unique<SharedMemory>();
    } else if (method == "grpc") {
        ipc = std::make_unique<GRPC>();
    } else if (method == "ucx") {
        ipc = std::make_unique<UCX>();
    } else {
        std::cerr << "Invalid IPC method: " << method << std::endl;
        return 1;
    }

    ipc->send(path);

    return 0;
}
