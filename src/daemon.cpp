#include <grpcpp/grpcpp.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server_builder.h>
#include "hello.grpc.pb.h"

class GreeterServiceImpl final : public hello::Greeter::Service {
    grpc::Status SayHello(grpc::ServerContext* context, const hello::HelloRequest* request, hello::HelloReply* response) override
    {
        std::cout << "[server] Received request: " << request->name() << std::endl;
        std::string prefix("Hello ");
        response->set_message(prefix + request->name());
        return grpc::Status::OK;
    }
};

void RunServer()
{
    const std::string server_address("0.0.0.0:50051");
    GreeterServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();
}

int main(void)
{
    RunServer();
    return 0;
}
