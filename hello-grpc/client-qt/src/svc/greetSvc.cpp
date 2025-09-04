#include "greetSvc.h"

// 构造函数初始化存根
greetSvc::greetSvc(std::shared_ptr<grpc::Channel> channel)
    : stub_(hello::Greeter::NewStub(channel)) {}

// 实现SayHello方法
std::string greetSvc::SayHello(const std::string& name) {
    hello::HelloRequest request;
    request.set_name(name); // 设置请求的name字段

    hello::HelloReply response;
    grpc::ClientContext context; // 创建客户端上下文

    // 发起RPC调用并获取状态
    grpc::Status status = stub_->SayHello(&context, request, &response);

    if (status.ok()) {
        return response.message(); // 成功返回响应消息
    } else {
        // 失败返回错误信息
        return "RPC failed: " + status.error_message() + " (" + std::to_string(status.error_code()) + ")";
    }
}