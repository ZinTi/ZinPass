#include "personal_mgr_form.h"
#include "ui_personal_mgr_form.h"

PersonalMgrForm::PersonalMgrForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonalMgrForm)
{
    ui->setupUi(this);
}

PersonalMgrForm::~PersonalMgrForm()
{
    delete ui;
}
