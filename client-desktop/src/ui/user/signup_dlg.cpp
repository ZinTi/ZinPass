#include <QMessageBox>
#include "signup_dlg.h"
#include "ui_signup_dlg.h"
#include "is_input_valid.h"
#include "user_rpc.h"
#include <string>

SignupDlg::SignupDlg(QWidget* parent) : QDialog(parent), ui(new Ui::SignupDlg) {
    ui->setupUi(this);
    this->ui->pBtnSubmit->setDefault(true);
}

SignupDlg::~SignupDlg() {
    delete ui;
}

void SignupDlg::on_pBtnSubmit_clicked() {
    // 1. 获取控件输入并初步检验数据合法性
    const QString inUsername = ui->editUsername->text();
    const QString inPwd1 = ui->editPwd1->text();
    const QString inPwd2 = ui->editPwd2->text();
    const QString inNickname = ui->editNickname->text();
    if (false == isUserIdValid(inUsername)) return;
    if (false == isPasswordValid(inPwd1, inPwd2)) return;
    if (false == isAliasValid(inNickname)) return;
    // 2. 执行注册操作
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::UserRPC user_rpc(channel);
    const auto[opt_data, message] = user_rpc.register_account(
        inUsername.toStdString(),
        inPwd1.toStdString(),
        inNickname.toStdString()
        );
    // 3. 弹窗显示结果
    if(opt_data == std::nullopt){
        QMessageBox::critical(this, QString("错误"), QString::fromStdString(message));
    } else {
        QMessageBox::information(this, QString("注册结果"), QString::fromStdString(message) + QString::fromStdString(", id: " + opt_data.value()));
    }
}

void SignupDlg::on_pBtnClear_clicked() const{
    ui->editNickname->clear();
    ui->editUsername->clear();
    ui->editPwd1->clear();
    ui->editPwd2->clear();
}

