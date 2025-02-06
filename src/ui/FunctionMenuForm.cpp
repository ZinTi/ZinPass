#include "FunctionMenuForm.h"
#include "ui_FunctionMenuForm.h"

FunctionMenuForm::FunctionMenuForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FunctionMenuForm)
{
    ui->setupUi(this);

    this->mascotLabel = new QLabel(this); // 创建 QLabel 用于显示 mascot
    displayMascot(20, 460, 72);
}

FunctionMenuForm::~FunctionMenuForm(){
    delete ui;
}


void FunctionMenuForm::on_pBtnPwd_clicked(){
    // 发出自定义信号，携带按钮索引 1
    emit buttonClicked(1);
}
void FunctionMenuForm::on_pBtnTele_clicked(){
    // 发出自定义信号，携带按钮索引 2
    emit buttonClicked(2);

}
void FunctionMenuForm::on_pBtnCheck_clicked(){
    emit buttonClicked(3);
}
void FunctionMenuForm::on_pBtnOwner_clicked(){
    emit buttonClicked(4);
}
void FunctionMenuForm::on_pBtnOut_clicked(){
    emit buttonClicked(5);
}
void FunctionMenuForm::on_pBtnIn_clicked(){
    emit buttonClicked(6);
}
void FunctionMenuForm::on_pBtnLogLogin_clicked(){
    emit buttonClicked(7);
}
void FunctionMenuForm::on_pBtnLogHandle_clicked(){
    emit buttonClicked(8);
}
void FunctionMenuForm::on_pBtnEditPersonal_clicked(){
    emit buttonClicked(9);
}
void FunctionMenuForm::on_pBtnCancelUser_clicked(){
    emit buttonClicked(10);
}
void FunctionMenuForm::on_pBtnExit_clicked(){
    emit buttonClicked(11);
}


void FunctionMenuForm::displayMascot(const int ax, const int ay, const int maxHeight) {
    const QPixmap pixmap(":/icon/mascot.png");
    if (!pixmap.isNull()) {
        int width = pixmap.width() * maxHeight / pixmap.height();
        if (width > 100) {
            width = 100;
        }
        mascotLabel->setFixedSize(width, maxHeight);
        // 使用平滑转换模式
        mascotLabel->setPixmap(pixmap.scaled(mascotLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        mascotLabel->move(ax, ay);
    }
    else {
        qDebug() << "Failed to load logo from resource.";
    }
}
