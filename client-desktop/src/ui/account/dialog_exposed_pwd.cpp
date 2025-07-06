#include "dialog_exposed_pwd.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QApplication>
#include <QClipboard>
#include "state_manager.h"
#include "account_rpc.h"

DialogExposedPwd::DialogExposedPwd(const std::string& account_id, QWidget* parent)
    : QDialog(parent), account_id_(account_id) {
    setup_ui();
}

DialogExposedPwd::~DialogExposedPwd() {

}

void DialogExposedPwd::setup_ui(){
    QVBoxLayout* layout_main = new QVBoxLayout(this);
    QFormLayout* layout_main_password = new QFormLayout;
    QHBoxLayout* layout_button = new QHBoxLayout;
    this->edit_display_ = new QTextEdit();
    this->edit_display_->setMaximumHeight(100);
    this->edit_display_->setReadOnly(true);
    this->edit_main_password_ = new QLineEdit();
    this->edit_main_password_->setEchoMode(QLineEdit::Password);
    this->edit_main_password_->setPlaceholderText(QString("主密码兼密钥"));
    this->btn_read_ = new QPushButton(QString("查看"));
    this->btn_copy_ = new QPushButton(QString("复制"));
    this->btn_read_and_copy_ = new QPushButton(QString("查看并复制"));
    this->btn_read_and_copy_->setDefault(true);
    layout_main_password->addRow(QString("密钥"), this->edit_main_password_);
    layout_button->addWidget(this->btn_read_);
    layout_button->addWidget(this->btn_copy_);
    layout_button->addWidget(this->btn_read_and_copy_);
    layout_main->setContentsMargins(QMargins(22,18,22,18));
    layout_main->addWidget(this->edit_display_);
    layout_main->addLayout(layout_main_password);
    layout_main->addLayout(layout_button);

    this->timer_ = new QTimer(this);
    connect(this->timer_, &QTimer::timeout, this, &DialogExposedPwd::update_button);

    connect(this->btn_read_, &QPushButton::clicked, this, &DialogExposedPwd::on_btn_read_clicked);
    connect(this->btn_copy_, &QPushButton::clicked, this, &DialogExposedPwd::on_btn_copy_clicked);
    connect(this->btn_read_and_copy_, &QPushButton::clicked, this, &DialogExposedPwd::on_btn_read_and_copy_clicked);
}

void DialogExposedPwd::start_btn_countdown(QPushButton* btn) {
    this->current_btn_ = btn;
    this->original_btn_text_ = btn->text();
    this->countdown_ = 6;
    this->current_btn_->setText(QString("%1 秒").arg(this->countdown_));
    this->btn_read_->setEnabled(false);
    this->btn_read_and_copy_->setEnabled(false);
    this->timer_->start(1000);
}

void DialogExposedPwd::update_button() {
    this->countdown_--;
    if (this->countdown_ > 0) {
        this->current_btn_->setText(QString("%1 秒").arg(this->countdown_));
    } else {
        this->timer_->stop();
        this->current_btn_->setText(this->original_btn_text_);
        this->btn_read_->setEnabled(true);
        this->btn_read_and_copy_->setEnabled(true);
        this->current_btn_ = nullptr;
        this->edit_display_->setTextColor(QColor::fromRgb(120, 120, 255));
        this->edit_display_->setText(QString("倒计时结束"));
    }
}

void DialogExposedPwd::on_btn_read_clicked() {
    this->edit_display_->clear();
    this->edit_display_->setTextColor(QColor::fromRgb(0, 255, 0));
    this->edit_display_->append(QString("[操作] 仅查看密码"));

    const std::string in_main_password = this->edit_main_password_->text().toStdString();
    this->edit_main_password_->clear(); // 读取后清除
    if(in_main_password.empty()){
        this->edit_display_->setTextColor(QColor::fromRgb(255, 0, 0));
        this->edit_display_->append(QString("[必需] 密钥为空，请输入主密码"));
        return;
    }

    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::AccountRPC account_rpc(channel);
    const auto[plaintext_password, message] = account_rpc.fetch_password(session_id, this->account_id_, in_main_password);

    if(plaintext_password.empty()){
        this->edit_display_->setTextColor(QColor::fromRgb(255, 0, 0));
        this->edit_display_->append(QString("[响应] %1").arg(message));
        this->edit_display_->append(QString("[密码] (空)"));
    }else{
        this->edit_display_->setTextColor(QColor::fromRgb(0, 255, 0));
        this->edit_display_->append(QString("[响应] %1").arg(message));
        this->edit_display_->append(QString("[密码] %1").arg(plaintext_password));

        start_btn_countdown(this->btn_read_);   // 成功获取到密码则开始倒计时
    }
}

void DialogExposedPwd::on_btn_copy_clicked(){
    this->edit_display_->clear();
    this->edit_display_->setTextColor(QColor::fromRgb(0, 255, 0));
    this->edit_display_->append(QString("[操作] 仅复制密码"));

    const std::string in_main_password = this->edit_main_password_->text().toStdString();
    this->edit_main_password_->clear(); // 读取后清除
    if(in_main_password.empty()){
        this->edit_display_->setTextColor(QColor::fromRgb(255, 0, 0));
        this->edit_display_->append(QString("[必需] 密钥为空，请输入主密码"));
        return;
    }

    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::AccountRPC account_rpc(channel);
    const auto[plaintext_password, message] = account_rpc.fetch_password(session_id, this->account_id_, in_main_password);

    if(plaintext_password.empty()){
        this->edit_display_->setTextColor(QColor::fromRgb(255, 0, 0));
        this->edit_display_->append(QString("[响应] %1").arg(message));
        this->edit_display_->append(QString("[复制] 未复制"));
    }else{
        this->edit_display_->setTextColor(QColor::fromRgb(0, 255, 0));
        this->edit_display_->append(QString("[响应] %1").arg(message));
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(QString::fromStdString(plaintext_password));
        this->edit_display_->append(QString("[复制] 已复制到剪贴板"));
    }
}

void DialogExposedPwd::on_btn_read_and_copy_clicked(){
    this->edit_display_->clear();
    this->edit_display_->setTextColor(QColor::fromRgb(0, 255, 0));
    this->edit_display_->append(QString("[操作] 查看密码并复制到剪贴板"));

    const std::string in_main_password = this->edit_main_password_->text().toStdString();
    this->edit_main_password_->clear(); // 读取后清除
    if(in_main_password.empty()){
        this->edit_display_->setTextColor(QColor::fromRgb(255, 0, 0));
        this->edit_display_->append(QString("[必需] 密钥为空，请输入主密码"));
        return;
    }

    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::AccountRPC account_rpc(channel);
    const auto[plaintext_password, message] = account_rpc.fetch_password(session_id, this->account_id_, in_main_password);

    if(plaintext_password.empty()){
        this->edit_display_->setTextColor(QColor::fromRgb(255, 0, 0));
        this->edit_display_->append(QString("[响应] %1").arg(message));
        this->edit_display_->append(QString("[密码] (空)"));
        this->edit_display_->append(QString("[复制] 未复制"));
    }else{
        this->edit_display_->setTextColor(QColor::fromRgb(0, 255, 0));
        this->edit_display_->append(QString("[响应] %1").arg(message));
        this->edit_display_->append(QString("[密码] %1").arg(plaintext_password));
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(QString::fromStdString(plaintext_password));
        this->edit_display_->append(QString("[复制] 已复制到剪贴板"));
        start_btn_countdown(this->btn_read_and_copy_);  // 成功获取到密码则开始倒计时
    }
}
