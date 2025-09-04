#ifndef GREET_SVC_H
#define GREET_SVC_H

#include <string>
#include <grpcpp/grpcpp.h>
#include "hello.grpc.pb.h"

class greetSvc {
public:
    // 构造函数，接受一个gRPC通道的共享指针来初始化存根
    greetSvc(std::shared_ptr<grpc::Channel> channel);

    // 调用远程服务的公共方法
    std::string SayHello(const std::string& name);

private:
    // Greeter服务的存根，用于发起RPC调用
    std::unique_ptr<hello::Greeter::Stub> stub_;
};

#endif // GREET_SVC_H