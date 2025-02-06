#include "CancelPersonalUserForm.h"
#include "ui_CancelPersonalUserForm.h"

CancelPersonalUserForm::CancelPersonalUserForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CancelPersonalUserForm)
{
    ui->setupUi(this);
}

CancelPersonalUserForm::~CancelPersonalUserForm()
{
    delete ui;
}
