#include "ucx.h"
#include <iostream>

void UCX::send(const std::string& path)
{
    std::cout << "UCX send: " << path << std::endl;
}

void UCX::receive()
{
    std::cout << "UCX receive" << std::endl;
}
