#pragma once
#include <string>
#include <vector>

class IPCBase {
public:
    virtual ~IPCBase() = default;
    virtual void send(const std::string& path) = 0;
    virtual void receive() = 0;
};
