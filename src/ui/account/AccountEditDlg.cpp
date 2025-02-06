#include "AccountEditDlg.h"
#include "AccountService.h"
#include "MobilePhoneService.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include "AuthDlg.h"

AccountEditDlg::AccountEditDlg(const int accountId, QWidget* parent) : QDialog(parent), ui(new Ui::AccountDlg),
    m_accountId(accountId) {
    ui->setupUi(this);
    setWindowTitle("修改账号密码信息");

    this->pBtnReset = new QPushButton("还原更改", this);
    this->pBtnSubmit = new QPushButton("提交更新", this);

    // 创建一个水平布局管理器
    auto* buttonLayout = new QHBoxLayout();
    // 将按钮添加到布局中
    buttonLayout->addWidget(pBtnReset);
    buttonLayout->addWidget(pBtnSubmit);

    // 将布局添加到窗口中，这里假设窗口中存在一个名为 verticalLayout 的垂直布局，可根据实际情况修改
    ui->verticalLayout->addLayout(buttonLayout);

    // 连接按钮的 clicked 信号到相应的槽函数
    connect(this->pBtnReset, &QPushButton::clicked, this, &AccountEditDlg::on_pBtnReset_clicked);
    connect(this->pBtnSubmit, &QPushButton::clicked, this, &AccountEditDlg::on_pBtnSubmit_clicked);

    initialData(); // 获取数据并加载到控件中

    // delete buttonLayout; 不能删
}

AccountEditDlg::~AccountEditDlg() {
    delete pBtnReset;
    delete pBtnSubmit;
    delete ui;
}

void AccountEditDlg::initialData() {
    // 1. 加载所有可选手机号和可选邮箱地址
    std::string msg1, msg2, msg3;
    std::vector<std::string> phoneOptions;
    std::vector<std::string> emailOptions;
    std::vector<std::string> categories;

    // 加载所有手机号
    const auto phoneService = new MobilePhoneService();
    phoneService->getPhoneOptions(msg1, phoneOptions);
    delete phoneService;
    ui->editPhone->clear(); // 清空原选项
    ui->editPhone->addItem(QString("无"));
    for (const std::string& option : phoneOptions) {
        ui->editPhone->addItem(QString::fromStdString(option));
    }

    // 加载所有邮箱
    const auto accountService = new AccountService();
    accountService->getEmailOptions(msg2, emailOptions);

    ui->editEmail->clear(); // 清空原选项
    ui->editEmail->addItem(QString("无"));
    for (const std::string& option : emailOptions) {
        ui->editEmail->addItem(QString::fromStdString(option));
    }

    // 加载所有类别
    accountService->getCategoryOptions(msg3, categories);
    ui->editCategory->clear(); // 清空原选项
    for (const std::string& option : categories) {
        ui->editCategory->addItem(QString::fromStdString(option));
    }

    // 2. 根据account.id获取目标数据
    if (accountService->getAccountById(msg3, m_accountId, m_viewAccount)) {
        on_pBtnReset_clicked(); // 手动加载下
    }
    else {
        QMessageBox::critical(this, QString("严重错误"), QString::fromStdString("找不到该条记录"));
        return;
    }
    delete accountService;
}

// 重置按钮的槽函数（初始化控件内容）
void AccountEditDlg::on_pBtnReset_clicked() const {
    // 服务商
    ui->editProvider->setText(QString::fromStdString(m_viewAccount.getProviderName()));
    // 平台名
    ui->editPlatform->setText(QString::fromStdString(m_viewAccount.getPlatformName()));
    // 网址
    ui->editUrl->setText(QString::fromStdString(m_viewAccount.getURL()));
    // 客服热线
    ui->editHotline->setText(QString::fromStdString(m_viewAccount.getHotline()));
    // 账号
    ui->editUserId->setText(QString::fromStdString(m_viewAccount.getUserId()));
    // 昵称
    ui->editAlias->setText(QString::fromStdString(m_viewAccount.getAlias()));
    // 子账号
    ui->editSubAccount->setText(QString::fromStdString(m_viewAccount.getSubAccount()));
    // 备注
    ui->editPostscript->setPlainText(QString::fromStdString(m_viewAccount.getPostscript()));

    // 绑定手机
    int countPhone = ui->editPhone->count(); // 获取 QComboBox 中的项数量
    for (int i = 0; i < countPhone; ++i) {
        if (ui->editPhone->itemText(i) == QString::fromStdString(m_viewAccount.getPhone())) {
            ui->editPhone->setCurrentIndex(i); // 找到匹配的项，将其设置为当前项
            break;
        }
    }

    // 绑定邮箱
    int countEmail = ui->editEmail->count();
    for (int i = 0; i < countEmail; ++i) {
        if (ui->editEmail->itemText(i) == QString::fromStdString(m_viewAccount.getEmail())) {
            ui->editEmail->setCurrentIndex(i); // 找到匹配的项，将其设置为当前项
            break;
        }
    }

    // 绑定类别
    int countCategory = ui->editCategory->count();
    for (int i = 0; i < countCategory; ++i) {
        if (ui->editCategory->itemText(i) == QString::fromStdString(m_viewAccount.getCategory())) {
            ui->editCategory->setCurrentIndex(i); // 找到匹配的项，将其设置为当前项
            break;
        }
    }

    ui->editPwd->setPlaceholderText(QString("新密码，置空不修改"));
}


// 更新按钮的槽函数
void AccountEditDlg::on_pBtnSubmit_clicked() {
    // 1. 获取用户输入、合法性检验
    QString inProvider = ui->editProvider->text();
    QString inPlatform = ui->editPlatform->text();
    QString inUrl = ui->editUrl->text();
    QString inHotline = ui->editHotline->text();
    QString inUserId = ui->editUserId->text();
    QString inAlias = ui->editAlias->text();
    QString inPwd = ui->editPwd->text();
    QString inPhone = ui->editPhone->currentText();
    QString inEmail = ui->editEmail->currentText();
    QString inSubAccount = ui->editSubAccount->text();
    QString inPostscript = ui->editPostscript->toPlainText();
    QString inCategory = ui->editCategory->currentText();

    // 2. 验证身份并获取主密码
    QString mainPwd;
    if(false == inPwd.isEmpty()){ // 不更新密码时，不用验证身份
        AuthDlg authDlg(this);
        if(authDlg.exec() == QDialog::Accepted){
            // 主密码
            mainPwd = authDlg.getInputMainPwd();
        }else{
            return;
        }
    }

    // 3. 更新记录
    const auto accountService = new AccountService();
    std::string feedbackMsg;
    if (accountService->updateAccount(feedbackMsg, mainPwd.toStdString(), this->m_accountId, inUserId.toStdString(), inAlias.toStdString(),
                                      inPwd.toStdString(), inSubAccount.toStdString(), inPhone.toStdString(),
                                      inEmail.toStdString(), inPostscript.toStdString(), inPlatform.toStdString(),
                                      inProvider.toStdString(), inUrl.toStdString(), inHotline.toStdString(), inCategory.toStdString())) {
        QMessageBox::information(this, QString("成功"), QString::fromStdString(feedbackMsg));
    }
    else {
        QMessageBox::warning(this, QString("失败"), QString::fromStdString(feedbackMsg));
    }
    delete accountService;
}
