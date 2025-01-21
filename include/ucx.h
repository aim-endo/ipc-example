#pragma once
#include "ipc_base.h"

class UCX : public IPCBase {
    void send(const std::string& path) override;
    void receive() override;
};
