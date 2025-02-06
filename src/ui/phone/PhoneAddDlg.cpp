#include "PhoneAddDlg.h"
#include "ui_PhoneAddDlg.h"
#include "TelecomOperatorService.h"
#include "MobilePhoneService.h"
#include <QMessageBox>

PhoneAddDlg::PhoneAddDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PhoneAddDlg)
{
    ui->setupUi(this);

    initialEditWidget(); // 初始化控件
}

PhoneAddDlg::~PhoneAddDlg()
{
    delete ui;
}

void PhoneAddDlg::initialEditWidget(){
    TelecomOperatorService teleOperService;
    std::string teleOperMsg;
    std::vector<std::string> teleOperators;
    teleOperService.getTelecomOperatorOptions(teleOperMsg, teleOperators);
    ui->editTeleOper->clear(); // 清除原有选项
    for(std::string option : teleOperators){ // 可选电信运营商
        ui->editTeleOper->addItem(QString::fromStdString(option));
    }

    ui->editJoinTime->setCalendarPopup(true);
    ui->editJoinTime->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
}


void PhoneAddDlg::on_pBtnClear_clicked(){
    ui->editPhone->clear();
    ui->editTeleOper->setCurrentIndex(0);
    ui->editServicePwd->clear();
    ui->editPin->clear();
    ui->editPuk->clear();
    ui->editJoinTime->clear();
    ui->editPhoneArea->clear();
    ui->editPostscript->clear();
}
void PhoneAddDlg::on_pBtnSubmit_clicked(){
    QString inPhone = ui->editPhone->text();
    QString inTeleOper = ui->editTeleOper->currentText();
    QString inServicePwd = ui->editServicePwd->text();
    QString inPin = ui->editPin->text();
    QString inPuk = ui->editPuk->text();
    QString inJoinTime = ui->editJoinTime->text();
    QString inPhoneArea = ui->editPhoneArea->text();
    QString inPostscript = ui->editPostscript->placeholderText();

    MobilePhoneService phoneService;
    std::string addMsg;
    if(
        phoneService.addMobilePhone(addMsg, inPhone.toStdString(), inTeleOper.toStdString(), inServicePwd.toStdString(), inPin.toStdString(), inPuk.toStdString(), inJoinTime.toStdString(),inPhoneArea.toStdString(),inPostscript.toStdString())
        ){
        QMessageBox::information(this,QString("成功"), QString::fromStdString("新增成功: " + addMsg));
    }else{
        QMessageBox::information(this,QString("失败"), QString::fromStdString("新增失败: " + addMsg));
    }
}
