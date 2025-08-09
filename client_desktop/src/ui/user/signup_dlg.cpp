#include <QMessageBox>
#include "signup_dlg.h"
#include "ui_signup_dlg.h"
#include "is_input_valid.h"
#include "user_rpc.h"
#include <string>

SignupDlg::SignupDlg(QWidget* parent) : QDialog(parent), ui(new Ui::SignupDlg) {
    ui->setupUi(this);
    ui->btn_submit_->setDefault(true);
}

SignupDlg::~SignupDlg() {
    delete ui;
}

void SignupDlg::on_btn_submit_clicked() {
    // 1. 获取控件输入并初步检验数据合法性
    const QString in_username = ui->e_username_->text();
    const QString in_pwd1 = ui->e_passwd1_->text();
    const QString in_pwd2 = ui->e_passwd2_->text();
    const QString in_nickname = ui->e_nickname_->text();
    if (false == isUserIdValid(in_username)) return;
    if (false == isPasswordValid(in_pwd1, in_pwd2)) return;
    if (false == isAliasValid(in_nickname)) return;
    // 2. 执行注册操作
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::UserRPC user_rpc(channel);
    const auto[opt_data, message] = user_rpc.register_account(
        in_username.toStdString(),
        in_pwd1.toStdString(),
        in_nickname.toStdString()
        );
    // 3. 弹窗显示结果
    if(opt_data == std::nullopt){
        QMessageBox::critical(this, QString("错误"), QString::fromStdString(message));
    } else {
        QMessageBox::information(this, QString("注册结果"), QString::fromStdString(message) + QString::fromStdString(", id: " + opt_data.value()));
    }
}

void SignupDlg::on_btn_clear_clicked() const{
    ui->e_nickname_->clear();
    ui->e_username_->clear();
    ui->e_passwd1_->clear();
    ui->e_passwd2_->clear();
}

