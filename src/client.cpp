#include <grpcpp/grpcpp.h>
#include <grpcpp/client_context.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/support/status.h>
#include <iostream>
#include <memory>

#include "hello.grpc.pb.h"

class GreeterClient final {
public:
    GreeterClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(hello::Greeter::NewStub(channel)) {}

    std::string SayHello(const std::string& name) {
        hello::HelloRequest request;
        request.set_name(name);

        hello::HelloReply reply;
        grpc::ClientContext context;

        grpc::Status status = stub_->SayHello(&context, request, &reply);

        if (status.ok())
            return reply.message();
        else {
            std::cerr << "RPC failed: " << status.error_message() << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<hello::Greeter::Stub> stub_;
};

int main(const int argc, const char* argv[])
{
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    GreeterClient client(channel);

    // send & receive
    const auto response = client.SayHello("world");
    std::cout << "Greeter Reply: " << response << std::endl;

    return 0;
}
