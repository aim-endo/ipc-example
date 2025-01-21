#pragma once
#include "ipc_base.h"

class GRPC : public IPCBase {
    void send(const std::string& path) override;
    void receive() override;
};
