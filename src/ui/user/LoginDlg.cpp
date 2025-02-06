#include "LoginDlg.h"
#include "ui_LoginDlg.h"
#include "Environ.h"
#include <QMessageBox>
#include "SystemUser.h"
#include "SystemUserService.h"
#include "SignupDlg.h"

extern Environ* PassGuardEnv;
extern SystemUser* currentSystemUser;

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
    ui->comboBoxUserId->clear(); // 清空项目
    // 获取userId列表
    std::string msg;
    std::vector<std::string> sysUserId;

    const auto sysUsrService = new SystemUserService();
    sysUsrService->getSysUserIdOptions(msg, sysUserId);
    delete sysUsrService;
    for (const std::string& str : sysUserId) { // 将 std::vector<std::string> 中的元素添加到 QComboBox 中
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
    // 获取用户名和输入框的内容
    const QString userId = ui->comboBoxUserId->currentText();
    const QString password = ui->lineEditPwd->text();

    std::string msg;
    const auto service = new SystemUserService();
    if (service->login(msg, userId.toStdString(), password.toStdString(), *currentSystemUser)) {
        accept(); // 登录成功，关闭登录对话框
    }
    else {
        QMessageBox::warning(this, QString("登录失败"), QString::fromStdString(msg)); // 登录失败，显示错误消息
    }
    delete service;
}

void LoginDlg::on_pBtnSignup_clicked() {
    SignupDlg signupDlg(this);
    signupDlg.exec();
    refreshComboBoxUserId();
}
