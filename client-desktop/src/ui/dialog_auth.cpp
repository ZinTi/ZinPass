#include "dialog_auth.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
// #include "authentication.h"

DialogAuth::DialogAuth(QWidget* parent) : QDialog(parent) {
    setWindowTitle("身份验证");
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
    connect(this->btn_okay_, &QPushButton::clicked, this, &DialogAuth::on_btn_okay_clicked);
    connect(this->btn_cancel_, &QPushButton::clicked, this, &QDialog::reject);
}

DialogAuth::~DialogAuth(){
    if(this->edit_password_){
        delete this->edit_password_;
    }
    if(this->btn_okay_){
        delete this->btn_okay_;
    }
    if(this->btn_cancel_){
        delete this->btn_cancel_;
    }
}

void DialogAuth::on_btn_okay_clicked() {
    std::string msg;
    this->main_password_ = this->edit_password_->text();
    // 验证当前用户身份
    /*
    const auto auth = new zinpass::business::Authentication();
    if (auth->validateUser(msg, this->main_password_.toStdString())) {
        accept();
    }
    else {
        QMessageBox::warning(this, QString("验证失败"), QString("密码错误"));
    }
    delete sysUsrMgr;*/
}

QString DialogAuth::get_input_main_password() const{
    return this->main_password_;
}
