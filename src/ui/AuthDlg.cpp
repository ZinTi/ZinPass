#include "AuthDlg.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include "SystemUserService.h"

AuthDlg::AuthDlg(QWidget* parent) : QDialog(parent) {
    setWindowTitle("身份验证");
    setFixedSize(200, 120);

    // 垂直布局管理器
    auto* mainLayout = new QVBoxLayout(this);

    // 密码标签和输入框
    auto* passwordLayout = new QHBoxLayout();
    auto* passwordLabel = new QLabel("口令:", this);
    editPwd = new QLineEdit(this);
    editPwd->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(editPwd);
    mainLayout->addLayout(passwordLayout);

    // 按钮布局
    auto* buttonLayout = new QHBoxLayout();
    auto* pBtnOk = new QPushButton("确定", this);
    auto* pBtnCancel = new QPushButton("取消", this);
    buttonLayout->addWidget(pBtnOk);
    buttonLayout->addWidget(pBtnCancel);
    mainLayout->addLayout(buttonLayout);

    // 连接按钮的点击信号到槽函数
    connect(pBtnOk, &QPushButton::clicked, this, &AuthDlg::on_pBtnOk_clicked);
    connect(pBtnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void AuthDlg::on_pBtnOk_clicked() {
    std::string msg;
    m_mainPwd = editPwd->text();
    const auto sysUsrService = new SystemUserService();
    if (sysUsrService->validateCurrentUser(msg, m_mainPwd.toStdString())) {
        accept();
    }
    else {
        QMessageBox::warning(this, QString("验证失败"), QString("密码错误"));
    }
    delete sysUsrService;
}

QString AuthDlg::getInputMainPwd() const{
    return m_mainPwd;
}
