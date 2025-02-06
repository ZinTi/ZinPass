#include "InputForm.h"
#include "ui_InputForm.h"

InputForm::InputForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InputForm)
{
    ui->setupUi(this);
}

InputForm::~InputForm()
{
    delete ui;
}
