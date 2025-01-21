#include "grpc.h"
#include <iostream>

void GRPC::send(const std::string& path)
{
    std::cout << "GRPC send: " << path << std::endl;
}

void GRPC::receive()
{
    std::cout << "GRPC receive" << std::endl;
}
