#include "ipc_base.h"
#include "shared_memory.h"
#include "grpc.h"
#include "ucx.h"
#include <iostream>
#include <memory>

int main(void)
{
    std::unique_ptr<IPCBase> ipc = std::make_unique<SharedMemory>();
    ipc->receive();
    return 0;
}
