#include <unistd.h>
#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using hello::HelloRequest;
using hello::HelloReply;
using hello::HelloService;
using hello::ByeByeRequest;
using hello::ByeByeResponse;

class HelloClient {
 public:
  HelloClient(std::shared_ptr<Channel> channel)
      : stub_(HelloService::NewStub(channel)) {}

  // Assambles the client's payload, sends it and presents the response back
  // from the server.
  std::string Hello(const std::string& user) {
    // Data we are sending to the server.
    HelloRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->Hello(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.reply();
    } else {
      return "RPC failed";
    }
  }    

    std::string Goodbye(const std::string &user) {
        ByeByeRequest request;
        request.set_name(user);

        ByeByeResponse response;

        ClientContext context;

        Status status = stub_->Goodbye(&context,request,&response);
        // Act upon its status.
        if (status.ok()) {
            return response.reply();
        } else {
            return "RPC failed";
        }
    }
        


 private:
  std::unique_ptr<HelloService::Stub> stub_;
};

int main(int argc, char** argv) {
    std::string host="localhost";
    std::string port = "5000";
    std::string user = "me";

    int c = 0;
    while ((c = getopt(argc,argv,"h:p:u:")) != EOF) {
        switch(c) {
        case 'h':
            host = optarg;
            break;
        case 'p':
            port = optarg;
            break;
        case 'u':
            user = optarg;
            break;
        default:
            break;
        }
    }
    

    HelloClient client(grpc::CreateChannel(host+":"+port,
                                           grpc::InsecureChannelCredentials()));
    std::string reply = client.Hello(user);
    std::cout << "Hello received: " << reply << std::endl;

    std::string reply2 = client.Goodbye(user);
    std::cout << "ByeBye received: " << reply << std::endl;

    return 0;
}
