#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>
#include <grpc++/security/server_credentials.h>

#include "helloworld.grpc.pb.h"
#include "ssl_test_data.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerCredentials;
using grpc::SslServerCredentials;
using grpc::SslServerCredentialsOptions;
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

void RunServer(const std::string &host, const std::string &port,
               const char*root,
               const char*cert, const char*key) {
    std::string server_address = host + ":" + port;

    HelloServiceImpl service;

    ServerBuilder builder;

    std::shared_ptr<ServerCredentials> creds;

    SslServerCredentialsOptions::PemKeyCertPair pkcp={ key, cert };
    SslServerCredentialsOptions ssl_opts;
    ssl_opts.pem_root_certs="";
    ssl_opts.pem_key_cert_pairs.push_back(pkcp);
    ssl_opts.pem_root_certs = root;
    creds = grpc::SslServerCredentials(ssl_opts);
    
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, creds);
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
    std::string rootFileName="../certs/ca.crt";
    std::string certFileName="../certs/server.crt";
    std::string keyFileName="../certs/server.key";

    int c = 0;
    while ((c = getopt(argc,argv,"h:p:r:c:k:")) != EOF) {
        switch(c) {
        case 'h':
            host = optarg;
            break;
        case 'p':
            port = optarg;
            break;
        case 'r':
            rootFileName=optarg;
            break;
        case 'c':
            certFileName=optarg;
            break;
        case 'k':
            keyFileName=optarg;
            break;
        }
    }

    std::stringstream caStr;
    std::stringstream certStr;
    std::stringstream keyStr;

    std::ifstream caFile(rootFileName);
    if (caFile) {
        caStr << caFile.rdbuf();
        caFile.close();
        std::cout << "Using root CA\n" << caStr.str().c_str() << std::endl;
    }

    std::ifstream certFile(certFileName);
    if (certFile) {
        certStr << certFile.rdbuf();
        certFile.close();

        std::cout << "Using cert:\n" << certStr.str().c_str() << std::endl;
    }

    std::ifstream keyFile(keyFileName);
    if (keyFile) {
        keyStr << keyFile.rdbuf();
        keyFile.close();

        std::cout << "Using key:\n" << keyStr.str().c_str() << std::endl;        
    }
    
    RunServer(host,port,caStr.str().c_str(),certStr.str().c_str(),keyStr.str().c_str());

    return 0;
}
