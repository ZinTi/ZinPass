#include "PhoneMgrForm.h"

#include <MobilePhone.h>

#include "ui_PhoneMgrForm.h"

#include <QMessageBox>
#include "MobilePhoneService.h"
#include "TelecomOperatorService.h"
#include "WidgetPublicMethod.h"
#include "PhoneAddDlg.h"
#include "AuthDlg.h"

PhoneMgrForm::PhoneMgrForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneMgrForm){
    ui->setupUi(this);

    if(false == ui->MsgDisplay->isReadOnly()) ui->MsgDisplay->setReadOnly(true);
    ui->tableWidget->setSortingEnabled(true); // 启用表头点击排序

    // 初始化筛选器和编辑器
    initialPhoneFilterAndEditor();

    // 连接信号和槽函数，用户点击表格项，获取行列坐标保存到私有属性中，并显示Id列信息
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &PhoneMgrForm::on_tableWidget_itemClicked);
}

PhoneMgrForm::~PhoneMgrForm(){
    delete ui;
}

bool PhoneMgrForm::getAllTeleOperatorOptions(){
    TelecomOperatorService teleOperService;
    std::string teleOperMsg;
    this->m_teleOperators.clear(); // 清空原有数据
    return teleOperService.getTelecomOperatorOptions(teleOperMsg, this->m_teleOperators);
}

bool PhoneMgrForm::getAllMobilePhoneOptions(){
    std::string msg;
    MobilePhoneService phoneService;
    this->m_phoneNumbers.clear(); // 清空原有数据
    return phoneService.getPhoneOptions(msg, this->m_phoneNumbers);
}

void PhoneMgrForm::initialPhoneFilterAndEditor() {
    // 获取手机号运营商可选项、手机号可选项
    getAllTeleOperatorOptions();
    getAllMobilePhoneOptions();

    // 1\ 筛选器
    // 填充所有运营商
    ui->cBoxTelecomOperator->clear(); // 清除原有选项
    ui->cBoxTelecomOperator->addItem(QString("所有")); // 默认不筛选
    for(std::string option : this->m_teleOperators){ // 填充电信运营商可选项
        ui->cBoxTelecomOperator->addItem(QString::fromStdString(option));
    }

    // 填充所有手机号
    ui->cBoxPhoneNumber->clear(); // 清空原选项
    ui->cBoxPhoneNumber->addItem(QString("所有")); // 默认不筛选
    for (const std::string& str : this->m_phoneNumbers) {
        ui->cBoxPhoneNumber->addItem(QString::fromStdString(str));
    }

    // 2\ 编辑器
    ui->editId->setEnabled(false);

    ui->editTeleOper->clear(); // 清除原有选项
    for(std::string option : this->m_teleOperators){ // 填充电信运营商可选项
        ui->editTeleOper->addItem(QString::fromStdString(option));
    }

    ui->editJoinTime->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    ui->editJoinTime->setCalendarPopup(true); // 开启日历弹出功能
}

void PhoneMgrForm::on_pBtnClear_clicked(){
    initialPhoneFilterAndEditor();
}
void PhoneMgrForm::on_pBtnQuery_clicked(){
    // 0. 定义变量
    QString inTelecomOperator = ui->cBoxTelecomOperator->currentText();
    QString inPhoneNumber = ui->cBoxPhoneNumber->currentText();
    // 1. 查询数据
    std::string msg;
    const auto phoneService = new MobilePhoneService();
    std::vector<MobilePhone> mobilePhones;
    phoneService->getMobilePhones(msg, inPhoneNumber.toStdString(), inTelecomOperator.toStdString(), mobilePhones);
    delete phoneService;

    int numRows = static_cast<int>(mobilePhones.size());

    // 2. 将查询到的数据填充到表格中显示
    ui->tableWidget->setRowCount(numRows);
    ui->tableWidget->setColumnCount(11);
    QStringList headers = {"id", "手机号", "运营商", "服务密码", "PIN", "PUK", "入网时间", "归属地", "所属用户", "创建时间", "更新时间"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < numRows; ++i) {
        const MobilePhone& info = mobilePhones[i];
        // 设置“ID”列
        auto idItem = new QTableWidgetItem(QString::fromStdString(std::to_string(info.getId())));
        ui->tableWidget->setItem(i, 0, idItem);

        // 设置手机号
        auto phoneItem = new QTableWidgetItem(QString::fromStdString(info.getPhoneNumber()));
        ui->tableWidget->setItem(i, 1, phoneItem);

        // 设置运营商
        auto telecomOperatorItem = new QTableWidgetItem(QString::fromStdString(info.getTelecomOperator()));
        ui->tableWidget->setItem(i, 2, telecomOperatorItem);

        // 设置服务密码
        auto servicePwdItem = new QTableWidgetItem(QString::fromStdString(info.getServicePwd()));
        ui->tableWidget->setItem(i, 3, servicePwdItem);

        // 设置PIN
        auto pinItem = new QTableWidgetItem(QString::fromStdString(info.getPin()));
        ui->tableWidget->setItem(i, 4, pinItem);

        // 设置PUK
        auto pukItem = new QTableWidgetItem(QString::fromStdString(info.getPuk()));
        ui->tableWidget->setItem(i, 5, pukItem);

        // 设置入网时间
        auto joinTimeItem = new QTableWidgetItem(QString::fromStdString(info.getJoinTime()));
        ui->tableWidget->setItem(i, 6, joinTimeItem);

        // 设置归属地
        auto phoneAreaItem = new QTableWidgetItem(QString::fromStdString(info.getPhoneArea()));
        ui->tableWidget->setItem(i, 7, phoneAreaItem);

        // 设置所属用户
        auto ownerItem = new QTableWidgetItem(QString::fromStdString((info.getSysUserId())));
        ui->tableWidget->setItem(i, 8, ownerItem);

        // 设置创建时间
        auto createdTimeItem = new QTableWidgetItem(QString::fromStdString(info.getCreatedTime()));
        ui->tableWidget->setItem(i, 9, createdTimeItem);

        // 设置修改时间
        auto updatedTimeItem = new QTableWidgetItem(QString::fromStdString(info.getUpdatedTime()));
        ui->tableWidget->setItem(i, 10, updatedTimeItem);
    }

    // 3、将所有列都设置为不可编辑
    for (int i = 0; i < 11; ++i) {
        setColumnNonEditable(ui->tableWidget, i);
    }

    // 4、在MsgDisplay中显示查询到的总记录数
    ui->MsgDisplay->append(QString::fromStdString("查询到 " + std::to_string(numRows) + " 条手机号记录\n"));


}
void PhoneMgrForm::on_pBtnRefresh_clicked(){
    QMessageBox::information(this, QString("未完成功能"), QString("重置"));
}
void PhoneMgrForm::on_pBtnAdd_clicked(){
    PhoneAddDlg phoneAddDlg(this);
    phoneAddDlg.exec();
}
void PhoneMgrForm::on_pBtnUpdate_clicked(){
    // 获取当前数据
    QString inId = ui->editId->text();
    QString inPhoneNumber = ui->editPhone->text();
    QString inTelecomOperator = ui->editTeleOper->currentText();
    QString inServicePwd = ui->editServPwd->text();
    QString inPin = ui->editPin->text();
    QString inPuk = ui->editPuk->text();
    QString inJoinTime = ui->editJoinTime->text();
    QString inPhoneArea = ui->editPhoneArea->text();
    QString inOwner = ui->editOwner->text();

    if(inId.isEmpty()){
        ui->MsgDisplay->append(QString::fromStdString("无效数据！请先查询并选中一条数据后再操作"));
        return;
    }

    // 更新数据
    MobilePhoneService* phoneService = new MobilePhoneService();
    std::string updateMsg;
    if(
        phoneService->updateMobilePhone(updateMsg, inId.toShort(), inPhoneNumber.toStdString(),
                                        inTelecomOperator.toStdString(), inServicePwd.toStdString(), inPin.toStdString(),
                                        inPuk.toStdString(),inJoinTime.toStdString(), inPhoneArea.toStdString(), inOwner.toStdString())
        ){
        ui->MsgDisplay->append(QString::fromStdString("更新成功! " + updateMsg));
    }else{
        ui->MsgDisplay->append(QString::fromStdString("更新失败! " + updateMsg));
    }
    delete phoneService;
}
void PhoneMgrForm::on_pBtnDelete_clicked(){
    // 0、获取删除目标
    QString inId = ui->editId->text();
    if(inId.isEmpty()){
        ui->MsgDisplay->append("无效数据！请先查询并选中一条数据后再操作");
        return;
    }
    short destId =  inId.toShort();
    AuthDlg authDlg;
    if (authDlg.exec() == QDialog::Accepted) {} else { return; } // 二次鉴权
    const int rowCount = ui->tableWidget->rowCount(); // 获取表格行数

    // 1、执行删除操作
    MobilePhoneService phoneService;
    std::string deleteMsg;
    if(
        phoneService.deleteMobilePhone(deleteMsg, destId)
        ){
        ui->MsgDisplay->append(QString::fromStdString("删除成功! " + deleteMsg));
    }else{
        ui->MsgDisplay->append(QString::fromStdString("删除失败! " + deleteMsg));
    }

    // 2、更新table控件
    // 删除用户后在ui->tableWidget中也删除那一行（相当于刷新操作，无需再次从数据库中查询）
    if(ui->tableWidget->item(row_tableW, 0)->text() == inId){ // 若还是这行（未因其他操作篡改）则直接删除
        ui->tableWidget->removeRow(row_tableW);
    }else{ // 否则找一下再删
        // bool found = false;
        for (int i = 0; i < rowCount; ++i) {
            if (ui->tableWidget->item(i, 0)->text() == inId) {
                ui->tableWidget->removeRow(i);
                // found = true;
                break;
            }
        }
    }
}

// 这里要改一下，不能从TableWidget中获取数据，而是获取id后从数据库查询
void PhoneMgrForm::on_tableWidget_itemClicked(const QTableWidgetItem *item) {
    // 当用户点击account表格中的项时会触发该信号，此槽函数用于处理信号
    if (item) {
        this->row_tableW = item->row();
        this->column_tableW = item->column();

        auto tableWidget = qobject_cast<QTableWidget*>(sender());
        if (tableWidget) {
            // ui->tableClickCoordViewer->setText(QString("r%1, c%2").arg(row_tableW).arg(column_tableW)); // 测试：显示坐标
            //ui->tableClickCoordViewer->setTextColor(QColor::fromRgbF(0, 255, 0, 1.0));
            //ui->tableClickCoordViewer->setText("已选中ID: " + tableWidget->item(row_tableW, 0)->text());
            //
            ui->editId->setText(tableWidget->item(row_tableW, 0)->text());
            ui->editPhone->setText(tableWidget->item(row_tableW, 1)->text());
            ui->editTeleOper->setCurrentText(tableWidget->item(row_tableW, 2)->text());
            ui->editServPwd->setText(tableWidget->item(row_tableW, 3)->text());
            ui->editPin->setText(tableWidget->item(row_tableW, 4)->text());
            ui->editPuk->setText(tableWidget->item(row_tableW, 5)->text());

            // 获取表格单元格的文本
            QTableWidgetItem *item = tableWidget->item(row_tableW, 6);
            if (item) {
                // 尝试将文本转换为 QDateTime 对象
                QDateTime joinTime = QDateTime::fromString(item->text(), QString("yyyy-MM-dd HH:mm:ss"));
                if (joinTime.isValid()) {
                    // 转换成功，设置到 QDateTimeEdit 控件中
                    ui->editJoinTime->setDateTime(joinTime);
                } else {
                    // 转换失败，先清除之前可能存储的有效值
                    ui->editJoinTime->clear();
                    ui->editJoinTime->setDateTime(QDateTime());
                    // 设置特殊值文本
                    ui->editJoinTime->setSpecialValueText("空");
                    // 让控件失去焦点，确保特殊值文本能正常显示
                    ui->editJoinTime->clearFocus();

                    // qDebug() << "日期时间转换失败，输入的文本是: " << item->text();
                }
            } else {
                // 单元格为空，先清除之前可能存储的有效值
                ui->editJoinTime->clear();
                ui->editJoinTime->setDateTime(QDateTime());
                // 设置特殊值文本
                ui->editJoinTime->setSpecialValueText("空");
                // 让控件失去焦点，确保特殊值文本能正常显示
                ui->editJoinTime->clearFocus();

                // qDebug() << "表格单元格为空，无法获取日期时间文本。";
            }

            ui->editPhoneArea->setText(tableWidget->item(row_tableW, 7)->text());
            ui->editOwner->setText(tableWidget->item(row_tableW, 8)->text());
        }
    }
}
