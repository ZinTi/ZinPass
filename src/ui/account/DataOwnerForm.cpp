#include "DataOwnerForm.h"
#include "ui_DataOwnerForm.h"

DataOwnerForm::DataOwnerForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DataOwnerForm)
{
    ui->setupUi(this);
}

DataOwnerForm::~DataOwnerForm()
{
    delete ui;
}
