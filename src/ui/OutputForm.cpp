#include "OutputForm.h"
#include "ui_OutputForm.h"

OutputForm::OutputForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OutputForm)
{
    ui->setupUi(this);
}

OutputForm::~OutputForm()
{
    delete ui;
}
