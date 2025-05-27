#include "dialog_delete_account.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include "state_manager.h"
#include "account_rpc.h"

DialogDeleteAccount::DialogDeleteAccount(const long long account_id, QWidget* parent) :account_id_(account_id), QDialog(parent) {
    setup_ui();
}

DialogDeleteAccount::~DialogDeleteAccount() {

}

void DialogDeleteAccount::set_account_id(const long long account_id){
    this->account_id_ = account_id;
}

std::string DialogDeleteAccount::get_input_main_password() const {
    return this->main_password_;
}

void DialogDeleteAccount::on_btn_okay_clicked(){
    this->main_password_ = this->edit_password_->text().toStdString();
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    // 执行删除操作
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::AccountRPC account_rpc(channel);
    const auto[result, message_remove] = account_rpc.remove_account(session_id, this->account_id_, this->main_password_);
    QMessageBox::information(this, QString("删除结果"), QString::fromStdString(message_remove));
    accept();
}

void DialogDeleteAccount::setup_ui(){
    setWindowTitle("删除账号");
    setFixedSize(200, 120);

    // 垂直布局管理器
    auto* layout_main = new QVBoxLayout(this);

    // 密码标签和输入框
    auto* layout_password = new QHBoxLayout();
    auto* label_password = new QLabel("主密码", this);
    this->edit_password_ = new QLineEdit(this);
    this->edit_password_->setEchoMode(QLineEdit::Password);
    layout_password->addWidget(label_password);
    layout_password->addWidget(this->edit_password_);
    layout_main->addLayout(layout_password);

    // 按钮布局
    auto* layout_button = new QHBoxLayout();
    this->btn_okay_ = new QPushButton("确定", this);
    this->btn_cancel_ = new QPushButton("取消", this);
    layout_button->addWidget(this->btn_okay_);
    layout_button->addWidget(this->btn_cancel_);
    layout_main->addLayout(layout_button);

    // 连接按钮的点击信号到槽函数
    connect(this->btn_okay_, &QPushButton::clicked, this, &DialogDeleteAccount::on_btn_okay_clicked);
    connect(this->btn_cancel_, &QPushButton::clicked, this, &QDialog::reject);
}
