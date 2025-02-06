#include "CheckPwdSecurityForm.h"
#include "ui_CheckPwdSecurityForm.h"

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
