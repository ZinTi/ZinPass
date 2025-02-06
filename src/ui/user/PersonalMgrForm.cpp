#include "PersonalMgrForm.h"
#include "ui_PersonalMgrForm.h"

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
