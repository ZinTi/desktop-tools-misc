#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QDateTime>
#include "greetSvc.h"
#include <grpcpp/grpcpp.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pBtnClear_clicked()
{
    this->ui->lineEditName->clear();
    this->ui->plainTextEditResponse->clear();
}


/**
 * @brief MainWindow::on_pBtnSubmit_clicked
 */
void MainWindow::on_pBtnSubmit_clicked()
{
    const QString name_of_input = this->ui->lineEditName->text();
    this->ui->plainTextEditResponse->appendPlainText( QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + QString(" | Name：") + name_of_input);

    // 创建gRPC通道连接到服务器
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    greetSvc svc(channel);

    // 调用服务并输出结果
    const std::string result = svc.SayHello(name_of_input.toStdString());
    this->ui->plainTextEditResponse->appendPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + QString(" | Response: ") + QString::fromStdString(result));
    this->ui->plainTextEditResponse->appendPlainText("-----------------------------------");
}

