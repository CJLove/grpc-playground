#include <unistd.h>
#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "helloworld.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using hello::HelloRequest;
using hello::HelloReply;
using hello::HelloService;
using hello::ByeByeRequest;
using hello::ByeByeResponse;

// Logic and data behind the server's behavior.
class HelloServiceImpl final : public HelloService::Service {
  Status Hello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello ");
    reply->set_reply(prefix + request->name());
    return Status::OK;
  }

  Status Goodbye(ServerContext* context, const ByeByeRequest* request,
                 ByeByeResponse* response) override {
      std::string prefix("Goodbye ");
      response->set_reply(prefix + request->name());
      return Status::OK;
  }
};

void RunServer(const std::string &host, const std::string &port) {
    std::string server_address = host + ":" + port;

    HelloServiceImpl service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv) {
    std::string host = "0.0.0.0";
    std::string port = "5000";

    int c = 0;
    while ((c = getopt(argc,argv,"h:p:")) != EOF) {
        switch(c) {
        case 'h':
            host = optarg;
            break;
        case 'p':
            port = optarg;
            break;
        }
    }

    RunServer(host,port);

    return 0;
}
