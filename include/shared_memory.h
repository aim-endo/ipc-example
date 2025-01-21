#pragma once
#include "ipc_base.h"

class SharedMemory : public IPCBase {
public:
    void send(const std::string& path) override;
    void receive() override;
};
