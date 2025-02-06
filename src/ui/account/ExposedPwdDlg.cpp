#include "ExposedPwdDlg.h"

#include "SystemUserService.h"
#include "AccountService.h"

#include "ui_ExposedPwdDlg.h"
#include "QMessageBox"

ExposedPwdDlg::ExposedPwdDlg(const int accountId, QWidget* parent)
    : QDialog(parent), ui(new Ui::ExposedPwdDlg), accountId(accountId) {
    ui->setupUi(this);
}

ExposedPwdDlg::~ExposedPwdDlg() {
    delete ui;
}


void ExposedPwdDlg::on_pBtnSubmit_clicked() {
    const std::string inMainPwd = ui->editSysUserPwd->text().toStdString();
    ui->editSysUserPwd->clear(); // 输入后清除口令
    std::string msgAuth; // 验证身份的反馈信息
    const auto sysUserService = new SystemUserService();
    if (sysUserService->validateCurrentUser(msgAuth, inMainPwd)) { // 验证身份成功
        const auto accountService = new AccountService();
        std::string msg;
        std::string pwd;
        if (accountService->getAccountPwd(msg, inMainPwd, this->accountId, pwd)) {
            ui->editPwdShow->setTextColor(QColor::fromRgbF(0, 255, 0, 1.0));
            ui->editPwdShow->setText(QString::fromStdString(pwd));
        }
        else {
            ui->editPwdShow->setTextColor(QColor::fromRgbF(255, 0, 0, 1.0));
            ui->editPwdShow->setText(QString::fromStdString(msg));
        }
        delete accountService;

        // accept();
    }
    else { // 验证身份失败
        ui->editPwdShow->setTextColor(QColor::fromRgbF(255, 0, 0, 1.0));
        ui->editPwdShow->setText(QString("口令错误"));
    }
    delete sysUserService;
}
