#include "AccountAddDlg.h"

#include <AccountService.h>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include "MobilePhoneService.h"
#include "AuthDlg.h"
//#include <QDebug>

AccountAddDlg::AccountAddDlg(QWidget* parent) : QDialog(parent), ui(new Ui::AccountDlg) {
    ui->setupUi(this);
    setWindowTitle("录入账号密码信息");

    this->pBtnClear = new QPushButton("清空输入", this);
    this->pBtnSubmit = new QPushButton("录入信息", this);

    // 创建一个水平布局管理器
    auto* buttonLayout = new QHBoxLayout();
    // 将按钮添加到布局中
    buttonLayout->addWidget(pBtnClear);
    buttonLayout->addWidget(pBtnSubmit);

    // 将布局添加到窗口中，这里假设窗口中存在一个名为 verticalLayout 的垂直布局，可根据实际情况修改
    ui->verticalLayout->addLayout(buttonLayout);

    // 连接按钮的 clicked 信号到相应的槽函数
    connect(this->pBtnClear, &QPushButton::clicked, this, &AccountAddDlg::on_pBtnClear_clicked);
    connect(this->pBtnSubmit, &QPushButton::clicked, this, &AccountAddDlg::on_pBtnSubmit_clicked);

    initialControl();

}

AccountAddDlg::~AccountAddDlg() {
    delete pBtnClear;
    delete pBtnSubmit;
    delete ui;
    // delete buttonLayout;
}

// 初始化输入控件
void AccountAddDlg::initialControl() const {
    // 1. 初始化控件内容

    std::string msg1, msg2, msg3;
    std::vector<std::string> phoneNumber;
    std::vector<std::string> emailAddress;
    std::vector<std::string> categories;

    // 加载所有手机号
    const auto phoneService = new MobilePhoneService();
    phoneService->getPhoneOptions(msg1, phoneNumber);
    delete phoneService;
    ui->editPhone->clear(); // 清空原选项
    ui->editPhone->addItem(QString("无"));
    for (const std::string& option : phoneNumber) {
        ui->editPhone->addItem(QString::fromStdString(option));
    }

    // 加载所有邮箱
    const auto accountService = new AccountService();
    accountService->getEmailOptions(msg2, emailAddress);
    ui->editEmail->clear(); // 清空原选项
    ui->editEmail->addItem(QString("无"));
    for (const std::string& option : emailAddress) {
        ui->editEmail->addItem(QString::fromStdString(option));
    }

    // 加载所有类别
    accountService->getCategoryOptions(msg3, categories);
    ui->editCategory->clear(); // 清空原选项
    for (const std::string& option : categories) {
        ui->editCategory->addItem(QString::fromStdString(option));
    }

    delete accountService;

    ui->editProvider->setPlaceholderText(QString("账号所属组织或公司"));
    ui->editPlatform->setPlaceholderText(QString("账号所属平台"));
    ui->editUrl->setPlaceholderText(QString("网站地址或IP地址"));
    ui->editHotline->setPlaceholderText(QString("客服电话"));
    ui->editUserId->setPlaceholderText(QString("UID/可用于登录的用户名"));
    ui->editAlias->setPlaceholderText(QString("昵称/实名"));
    ui->editPwd->setPlaceholderText(QString("该账号的密码"));
    ui->editSubAccount->setPlaceholderText(QString("是否有带二级密码的子账号"));
    ui->editPostscript->setPlaceholderText(QString("其他备注"));
}


// 重置按钮的槽函数
void AccountAddDlg::on_pBtnClear_clicked() const {
    initialControl(); // 刷新控件可选项、设置placeholder等

    ui->editProvider->clear();
    ui->editPlatform->clear();
    ui->editUrl->clear();
    ui->editHotline->clear();
    ui->editUserId->clear();
    ui->editAlias->clear();
    ui->editPwd->clear();
    ui->editSubAccount->clear();
    ui->editPostscript->clear();
}


// 录入按钮的槽函数
void AccountAddDlg::on_pBtnSubmit_clicked() {
    // 1. 获取用户输入、合法性检验
    const QString inProvider = ui->editProvider->text();
    const QString inPlatform = ui->editPlatform->text();
    const QString inUrl = ui->editUrl->text();
    const QString inHotline = ui->editHotline->text();
    const QString inUserId = ui->editUserId->text();
    const QString inAlias = ui->editAlias->text();
    const QString inPwd = ui->editPwd->text();
    const QString inPhone = ui->editPhone->currentText();
    const QString inEmail = ui->editEmail->currentText();
    const QString inSubAccount = ui->editSubAccount->text();
    const QString inPostscript = ui->editPostscript->toPlainText();
    const QString inCategory = ui->editCategory->currentText();

    // 2. 验证身份并获取主密码
    QString mainPwd;
    AuthDlg authDlg(this);
    if(authDlg.exec() == QDialog::Accepted){
        // 主密码
        mainPwd = authDlg.getInputMainPwd();
    }else{
        return;
    }

    // 3. 录入记录
    const auto accountService = new AccountService();
    std::string feedbackMsg;
    if (
            accountService->addAccount(feedbackMsg,
                                   mainPwd.toStdString(),
                                   inUserId.toStdString(),
                                   inAlias.toStdString(),
                                   inPwd.toStdString(),
                                   inSubAccount.toStdString(),
                                   inPhone.toStdString(),
                                   inEmail.toStdString(),
                                   inPostscript.toStdString(),
                                   inPlatform.toStdString(),
                                   inProvider.toStdString(),
                                   inUrl.toStdString(),
                                   inHotline.toStdString(),
                                   inCategory.toStdString()
            )

        ) {
        QMessageBox::information(this, QString("成功"), QString::fromStdString(feedbackMsg));
    }
    else {
        QMessageBox::warning(this, QString("失败"), QString::fromStdString(feedbackMsg));
    }
    delete accountService;
}
