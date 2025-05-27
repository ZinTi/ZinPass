#include "login_dlg.h"
#include <QMessageBox>
#include <state_manager.h>
#include "ui_login_dlg.h"
#include "signup_dlg.h"
#include "user.h"
#include "user_rpc.h"
#include "auth_rpc.h"
#include "state_manager.h"

LoginDlg::LoginDlg(QWidget* parent) : QDialog(parent), ui(new Ui::LoginDlg) {
    ui->setupUi(this);

    refreshComboBoxUserId(); // 刷新登录UserId列表

    this->logoLabel = new QLabel(this); // 创建 QLabel 用于显示 logo
    displayLogo(236, 100, 72);
}

LoginDlg::~LoginDlg() {
    delete logoLabel;
    delete ui;
}

void LoginDlg::refreshComboBoxUserId() const {
    ui->comboBoxUserId->clear();
    std::string msg;
    std::vector<std::string> usernames;

    // 创建gRPC通道连接到服务器
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::UserRPC user_rpc(channel);
    user_rpc.list_usernames(usernames);

    // 添加到组件中
    for (const std::string& str : usernames) {
        ui->comboBoxUserId->addItem(QString::fromStdString(str));
    }
}

void LoginDlg::displayLogo(const int ax, const int ay, const int maxHeight) {
    const QPixmap pixmap(":/icon/logo.png");
    if (!pixmap.isNull()) {
        int width = pixmap.width() * maxHeight / pixmap.height();
        if (width > 100) {
            width = 100;
        }
        logoLabel->setFixedSize(width, maxHeight);
        // 使用平滑转换模式
        logoLabel->setPixmap(pixmap.scaled(logoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        logoLabel->move(ax, ay);
    }
    else {
        qDebug() << "Failed to load logo from resource.";
    }
}

void LoginDlg::on_pBtnLogin_clicked() {
    auto user = zinpass::models::SystemUser(); // 用户
    // 获取用户名和输入框的内容
    const QString in_username = ui->comboBoxUserId->currentText();
    const QString in_password = ui->lineEditPwd->text();
    // const QString in_captcha_code = ui->lineEditCaptcha->text();

    // 执行登录操作
    // 创建gRPC通道连接到服务器
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::AuthRPC auth_rpc(channel);
    const std::map<std::string, std::string> res = auth_rpc.login(in_username.toStdString(), in_password.toStdString(), "captcha-code: 0123");

    // 反序列化 response
    zinpass::state::StateManager::UserState state;
    std::string message;

    try {
        state.session_id = res.at("session_id");
        state.user_id = res.at("user_id");
        state.username = res.at("username");
        state.nickname = res.at("nickname");
        // state.role = ;
    } catch (const std::out_of_range& e){
        QMessageBox::warning(this, QString("登录失败"), QString("会话建立失败: ") + QString::fromStdString(e.what()));
        return;
    }
    zinpass::state::StateManager::instance().setUserState(state);

    if(zinpass::state::StateManager::instance().getUserState().session_id.size() == 36){
        accept(); // 登录成功，关闭登录对话框
    }else{
        try{
            message = res.at("message");
        }catch(const std::out_of_range& e){
            message = "无消息";
        }

        QMessageBox::warning(this, QString("登录失败"), QString::fromStdString(message)); // 登录失败，显示错误消息
    }

}

void LoginDlg::on_pBtnSignup_clicked() {
    SignupDlg signupDlg(this);
    signupDlg.exec();
    refreshComboBoxUserId();
}
