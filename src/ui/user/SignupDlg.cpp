#include <QMessageBox>
#include "SignupDlg.h"

#include <SystemUserService.h>

#include "ui_SignupDlg.h"
#include "SystemUser.h"
#include "IsInputValid.h"

SignupDlg::SignupDlg(QWidget* parent) : QDialog(parent), ui(new Ui::SignupDlg) {
    ui->setupUi(this);
}

SignupDlg::~SignupDlg() {
    delete ui;
}

void SignupDlg::on_pBtnSubmit_clicked() {
    //
    const QString inAlias = ui->editAlias->text();
    const QString inSysUserId = ui->editUserId->text();
    const QString inPwd1 = ui->editPwd1->text();
    const QString inPwd2 = ui->editPwd2->text();
    //
    if (false == isAliasValid(inAlias)) return;
    if (false == isUserIdValid(inSysUserId)) return;
    if (false == isPasswordValid(inPwd1, inPwd2)) return;
    //
    SystemUser sysUser;
    sysUser.setAlias(inAlias.toStdString());
    sysUser.setUserId(inSysUserId.toStdString());
    sysUser.setPwd(inPwd1.toStdString());
    //
    std::string msg;
    const auto sysUsrService = new SystemUserService();
    if (sysUsrService->registerSysUser(msg, sysUser)) {
        QMessageBox::information(this, QString("注册完成"), QString::fromStdString(msg));
    }
    else {
        QMessageBox::warning(this, QString("注册失败"), QString::fromStdString(msg));
    }
    delete sysUsrService;
}

void SignupDlg::on_pBtnClear_clicked() const{
    ui->editAlias->clear();
    ui->editUserId->clear();
    ui->editPwd1->clear();
    ui->editPwd2->clear();
}

