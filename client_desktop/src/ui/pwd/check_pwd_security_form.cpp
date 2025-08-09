#include "check_pwd_security_form.h"
#include "ui_check_pwd_security_form.h"

CheckPwdSecurityForm::CheckPwdSecurityForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CheckPwdSecurityForm)
{
    ui->setupUi(this);
}

CheckPwdSecurityForm::~CheckPwdSecurityForm()
{
    delete ui;
}
