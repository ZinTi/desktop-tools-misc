use hello_world::greeter_client::GreeterClient;
use hello_world::HelloRequest;
use std::io;
use std::io::Write;

pub mod hello_world {
    tonic::include_proto!("hello");
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut client = GreeterClient::connect("http://[::1]:50051").await?;

    let mut index: i32 = 10;
    while index != 0 {
        index -= 1;

        print!("请输入您的姓名：");
        io::stdout().flush().unwrap();

        let mut name = String::new();
        io::stdin().read_line(&mut name)?;
        let name = name.trim().to_string();

        // 创建包含用户输入的请求
        let request = tonic::Request::new(HelloRequest {
            name: name.into(),
        });

        // 发送请求并获取响应
        let response = client.say_hello(request).await?;

        println!("RESPONSE={:?}", response);
    }

    Ok(())
}