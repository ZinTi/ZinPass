#include "AccountMgrForm.h"
#include "ui_AccountMgrForm.h"
#include "ExposedPwdDlg.h"
#include "IsInputValid.h"
#include "AccountAddDlg.h"
#include "AccountEditDlg.h"
#include "AuthDlg.h"
#include <QMessageBox>

#include "MobilePhoneService.h"
#include "AccountService.h"
#include "WidgetPublicMethod.h"
#include "PwdGenDlg.h"
// #include "PwdGeneratorDlg.h"

#include <QToolButton>

AccountMgrForm::AccountMgrForm(QWidget *parent) : QWidget(parent), ui(new Ui::AccountMgrForm){
    ui->setupUi(this);

    initialPopupFilterWindow(); // 初始化下拉筛选弹窗

    // QObject::connect(this->tBtnDropdownMore, &QToolButton::clicked, this, &AccountMgrForm::on_tBtnDropdownMore_clicked); // 处理下拉按钮点击事件
    // QObject::connect(ui->pBtnQuery, &QToolButton::clicked, this, &AccountMgrForm::on_pBtnQuery_clicked); // ui自动绑定，无需手动绑定
    //

    initialAccountFilter(); // 初始化账号筛选控件

    if(false == ui->coordViewer->isReadOnly()) ui->coordViewer->setReadOnly(true);
    ui->tableWidgetAccount->setSortingEnabled(true); // 启用表头点击排序

    // 连接信号和槽函数，用户点击表格项，获取行列坐标保存到私有属性中，并显示Id列信息
    connect(ui->tableWidgetAccount, &QTableWidget::itemClicked, this, &AccountMgrForm::on_tableWidgetAccount_itemClicked); // account表

}

AccountMgrForm::~AccountMgrForm(){
    delete ui;
}

void AccountMgrForm::initialPopupFilterWindow(){
    // 创建下拉按钮，使用QToolButton
    this->tBtnDropdownMore = new QToolButton(this);
    // this->tBtnDropdownMore->setText("更多筛选");
    this->tBtnDropdownMore->setPopupMode(QToolButton::InstantPopup); // 设置弹出模式
    this->tBtnDropdownMore->setArrowType(Qt::DownArrow); // 显示下拉箭头
    this->tBtnDropdownMore->move(760,150);

    this->popupWindow = new PopupFilterWindow(this);
    // this->popupWindow->setStyleSheet("background-color: lightgreen; color: #333333;");

    // 处理下拉按钮点击事件
    connect(this->tBtnDropdownMore, &QToolButton::clicked, [this]() {
        if (this->popupWindow->isHidden()) {
            QPoint pos = mapToGlobal(this->tBtnDropdownMore->pos());
            pos.setY(pos.y() + this->tBtnDropdownMore->height());
            this->popupWindow->move(pos); // 移动到下拉按键的下方
            this->popupWindow->show();
            // 给窗口设置了Qt::Popup属性后无法输入中文，强制更新焦点链
            this->popupWindow->activateWindow(); //强制激活popup窗口（焦点窗口）
        } else {
            this->popupWindow->hide();
        }
    });

    //QObject::connect(this->tBtnDropdownMore, &QToolButton::clicked, this, &AccountMgrForm::on_tBtnDropdownMore_clicked); // 处理下拉按钮点击事件

}

void AccountMgrForm::initialAccountFilter() const {
    QDateTime specifiedDateTime(QDate(2000, 1, 1), QTime(8, 0, 0)); // 创建一个 QDateTime 对象，设置为指定时间
    ui->uTimeRangeStart->setDateTime(specifiedDateTime);
    ui->uTimeRangeEnd->setDateTime(QDateTime::currentDateTime()); // 设置当前时间

    ui->uTimeRangeStart->setDisplayFormat("yyyy-MM-dd HH:mm:ss"); // 设置显示格式
    ui->uTimeRangeStart->setCalendarPopup(true); // 开启日历弹出功能
    ui->uTimeRangeEnd->setDisplayFormat("yyyy-MM-dd HH:mm:ss"); // 设置显示格式
    ui->uTimeRangeEnd->setCalendarPopup(true); // 开启日历弹出功能

    std::string msg1, msg2;
    std::vector<std::string> phoneNumber;
    std::vector<std::string> emailAddress;

    // 加载所有手机号
    const auto phoneService = new MobilePhoneService();
    phoneService->getPhoneOptions(msg1, phoneNumber);
    delete phoneService;
    ui->comboBoxPhone->clear(); // 清空原选项
    ui->comboBoxPhone->addItem(QString("所有")); // 默认选项
    ui->comboBoxPhone->addItem(QString("空值-暂未实现"));
    for (const std::string& str : phoneNumber) {
        ui->comboBoxPhone->addItem(QString::fromStdString(str));
    }

    // 加载所有邮箱
    const auto emailService = new AccountService();
    emailService->getEmailOptions(msg2, emailAddress);
    delete emailService;
    ui->comboBoxEmail->clear(); // 清空原选项
    ui->comboBoxEmail->addItem(QString("所有")); // 默认选项
    ui->comboBoxEmail->addItem(QString("空值-暂未实现"));
    for (const std::string& str : emailAddress) {
        ui->comboBoxEmail->addItem(QString::fromStdString(str));
    }
}

void AccountMgrForm::on_tBtnDropdownMore_clicked(){
    if (this->popupWindow->isHidden()) {
        QPoint pos = mapToGlobal(this->tBtnDropdownMore->pos());
        pos.setY(pos.y() + this->tBtnDropdownMore->height());
        this->popupWindow->move(pos); // 移动到下拉按键的下方
        this->popupWindow->show();
        // 给窗口设置了Qt::Popup属性后无法输入中文，强制更新焦点链
        this->popupWindow->activateWindow(); //强制激活popup窗口（焦点窗口）
    } else {
        this->popupWindow->hide();
    }
}

void AccountMgrForm::on_pBtnReset_clicked() const {
    initialAccountFilter(); // 重新初始化AccountFilter
    ui->editProvider->clear();
    ui->editPlatform->clear();
    ui->editUserId->clear();
    ui->editAlias->clear();
}

void AccountMgrForm::on_pBtnQuery_clicked() {
    // 0. 定义变量
    QString inProviderName;
    QString inPlatformName;
    QString inUserId;
    QString inAlias;
    QDateTime inUTimeRangeStart;
    QDateTime inUTimeRangeEnd;
    QString inEmail;
    QString inPhone;
    std::string providerName;
    std::string platformName;
    std::string userId;
    std::string alias;
    std::string uTimeRangeStart;
    std::string uTimeRangeEnd;
    std::string email;
    std::string phone;
    // 1. 获取输入并检查输入数据的合法性
    inProviderName = ui->editProvider->text();
    providerName = (inProviderName.isEmpty() ? "" : inProviderName.toStdString());
    inPlatformName = ui->editPlatform->text();
    platformName = (inPlatformName.isEmpty() ? "" : inPlatformName.toStdString());
    inUserId = ui->editUserId->text();
    userId = (inUserId.isEmpty() ? "" : inUserId.toStdString());
    inAlias = ui->editAlias->text();
    alias = (inAlias.isEmpty() ? "" : inAlias.toStdString());
    inUTimeRangeStart = ui->uTimeRangeStart->dateTime();
    inUTimeRangeEnd = ui->uTimeRangeEnd->dateTime();
    if (false == isUTimeRangeValid(inUTimeRangeStart, inUTimeRangeEnd)) {
        return;
    }
    else {
        uTimeRangeStart = inUTimeRangeStart.toString("yyyy-MM-dd HH:mm:ss").toStdString();
        uTimeRangeEnd = inUTimeRangeEnd.toString("yyyy-MM-dd HH:mm:ss").toStdString();
    }
    inEmail = ui->comboBoxEmail->currentText();
    email = (inEmail == "所有" ? "" : inEmail.toStdString()); // 所有、空值、数据库 email 可选项列表
    inPhone = ui->comboBoxPhone->currentText();
    phone = (inPhone == "所有" ? "" : inPhone.toStdString()); // 所有、空值、数据库 mobile phone 可选项列表
    // 2. 如果输入数据合法则查询数据，否则结束
    std::string msg;
    const auto accountService = new AccountService();
    std::vector<ViewAccount> viewAccounts;
    accountService->getAllAccounts(msg, providerName, platformName, userId, alias, phone, email, uTimeRangeStart,uTimeRangeEnd, viewAccounts);
    delete accountService;

    const unsigned int numRows = static_cast<unsigned int>(viewAccounts.size());

    // 3、将查询到的数据填充到表格中显示
    ui->tableWidgetAccount->setRowCount(numRows);
    ui->tableWidgetAccount->setColumnCount(14);
    QStringList headers = {"id", "服务商", "平台名", "账号", "昵称", "子账号", "绑定手机号", "绑定邮箱", "备注", "网址", "客服热线", "类别", "创建时间", "更新时间"};
    ui->tableWidgetAccount->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < numRows; ++i) {
        const ViewAccount& info = viewAccounts[i];
        // 设置 "ID" 列
        auto idItem = new QTableWidgetItem(QString::fromStdString(std::to_string(info.getId())));
        ui->tableWidgetAccount->setItem(i, 0, idItem); // 行，列，值

        // 设置 "服务商" 列
        auto providerNameItem = new QTableWidgetItem(QString::fromStdString(info.getProviderName()));
        ui->tableWidgetAccount->setItem(i, 1, providerNameItem);

        // 设置 "平台名" 列
        auto platformNameItem = new QTableWidgetItem(QString::fromStdString(info.getPlatformName()));
        ui->tableWidgetAccount->setItem(i, 2, platformNameItem);

        // 设置 "账号ID" 列
        auto userIdItem = new QTableWidgetItem(QString::fromStdString(info.getUserId()));
        ui->tableWidgetAccount->setItem(i, 3, userIdItem);

        // 设置 "昵称" 列
        auto aliasItem = new QTableWidgetItem(QString::fromStdString(info.getAlias()));
        ui->tableWidgetAccount->setItem(i, 4, aliasItem);

        // 设置 "子账号" 列
        auto subAccountItem = new QTableWidgetItem(QString::fromStdString(info.getSubAccount()));
        ui->tableWidgetAccount->setItem(i, 5, subAccountItem);

        // 设置 "绑定手机号" 列
        auto telephoneItem = new QTableWidgetItem(QString::fromStdString(info.getPhone()));
        ui->tableWidgetAccount->setItem(i, 6, telephoneItem); // 行，列，值

        // 设置 "绑定邮箱" 列
        auto emailItem = new QTableWidgetItem(QString::fromStdString(info.getEmail()));
        ui->tableWidgetAccount->setItem(i, 7, emailItem);

        // 设置 "备注" 列
        auto postscriptItem = new QTableWidgetItem(QString::fromStdString(info.getPostscript()));
        ui->tableWidgetAccount->setItem(i, 8, postscriptItem);

        // 设置 "网址" 列
        auto urlItem = new QTableWidgetItem(QString::fromStdString(info.getURL()));
        ui->tableWidgetAccount->setItem(i, 9, urlItem);

        // 设置 "客服热线" 列
        auto hotlineItem = new QTableWidgetItem(QString::fromStdString(info.getHotline()));
        ui->tableWidgetAccount->setItem(i, 10, hotlineItem);

        // 设置 "类别" 列
        auto categoryItem = new QTableWidgetItem(QString::fromStdString(info.getCategory()));
        ui->tableWidgetAccount->setItem(i, 11, categoryItem);

        // 设置 "创建时间" 列
        auto createdTimeItem = new QTableWidgetItem(QString::fromStdString(info.getCreatedTime()));
        ui->tableWidgetAccount->setItem(i, 12, createdTimeItem);

        // 设置 "修改时间" 列
        auto updatedTimeItem = new QTableWidgetItem(QString::fromStdString(info.getUpdatedTime()));
        ui->tableWidgetAccount->setItem(i, 13, updatedTimeItem);
    }

    // 4、将所有列(0-13)都设置为不可编辑
    for (int i = 0; i <= 13; i++) {
        setColumnNonEditable(ui->tableWidgetAccount, i);
    }

    // 5、默认选中(0,0)项
    if (ui->tableWidgetAccount->rowCount() <= 0) {
        ui->coordViewer->setTextColor(QColor::fromRgbF(255, 100, 0, 1.0));
        ui->coordViewer->setText("没有找到符合的记录");
    }
    else {
        row_tableW = 0;
        column_tableW = 0;
        ui->coordViewer->setTextColor(QColor::fromRgbF(0, 100, 255, 1.0));
        ui->coordViewer->setText("默认选中ID: " + ui->tableWidgetAccount->item(row_tableW, 0)->text());
    }
}

void AccountMgrForm::on_pBtnShow_clicked() { // 查看密码弹窗
    QString destId;
    const int rowCount = ui->tableWidgetAccount->rowCount();
    if (row_tableW < rowCount) {
        destId = ui->tableWidgetAccount->item(row_tableW, 0)->text();
    }
    else {
        ui->coordViewer->setTextColor(QColor::fromRgbF(255, 100, 0, 1.0));
        ui->coordViewer->setText("表格中无数据！请先查询");
        return;
    }
    ExposedPwdDlg exposedPwdDialog(destId.toInt(), this);
    exposedPwdDialog.setWindowTitle(QString("查看密码( ") + destId + QString(" )"));
    exposedPwdDialog.exec();
}


void AccountMgrForm::on_pBtnAdd_clicked() {
    AccountAddDlg accountAddDlg(this);
    accountAddDlg.exec();
}

void AccountMgrForm::on_pBtnUpdate_clicked() {
    QString destId;
    const int rowCount = ui->tableWidgetAccount->rowCount();
    if (row_tableW < rowCount) {
        destId = ui->tableWidgetAccount->item(row_tableW, 0)->text();
    }
    else {
        ui->coordViewer->setTextColor(QColor::fromRgbF(255, 100, 0, 1.0));
        ui->coordViewer->setText("表格中无数据！请先查询");
        return;
    }
    AccountEditDlg accountEditDlg(destId.toInt(), this);
    accountEditDlg.setWindowTitle(QString("修改账号密码信息( ") + destId + QString(" )"));
    accountEditDlg.exec();
}

void AccountMgrForm::on_pBtnDelete_clicked() {
    QString destId;
    const int rowCount = ui->tableWidgetAccount->rowCount();
    if (row_tableW < rowCount) {
        destId = ui->tableWidgetAccount->item(row_tableW, 0)->text();
    }
    else {
        ui->coordViewer->setTextColor(QColor::fromRgbF(255, 100, 0, 1.0));
        ui->coordViewer->setText("表格中无数据！请先查询");
        return;
    }
    AuthDlg authDlg;
    if (authDlg.exec() == QDialog::Accepted) {} else { return; } // 二次鉴权

    const auto accountService = new AccountService();
    std::string msg;
    const bool result = accountService->deleteAccount(msg, destId.toInt());
    delete accountService;
    if(result){
        QMessageBox::information(this, QString::fromStdString("删除账号" + destId.toStdString() +"成功"), QString::fromStdString(msg));
    }else{
        QMessageBox::warning(this, QString::fromStdString("删除账号" + destId.toStdString() + "失败"), QString::fromStdString(msg));
    }
    // 删除用户后在ui->tableWidget中也删除那一行（相当于刷新操作，无需再次从数据库中查询）
    if(ui->tableWidgetAccount->item(row_tableW, 0)->text() == destId){ // 若还是这行（未因其他操作篡改）则直接删除
        ui->tableWidgetAccount->removeRow(row_tableW);
    }else{ // 否则找一下再删
        // bool found = false;
        for (int i = 0; i < rowCount; ++i) {
            if (ui->tableWidgetAccount->item(i, 0)->text() == destId) {
                ui->tableWidgetAccount->removeRow(i);
                // found = true;
                break;
            }
        }
    }
}

// 随机密码生成器-toolBtn
void AccountMgrForm::on_tBtnPwdGenerator_clicked(){
    PwdGenDlg pwdG(this);
    //PwdGeneratorDlg pwdG(this);
    pwdG.exec();

}

void AccountMgrForm::on_tableWidgetAccount_itemClicked(const QTableWidgetItem* item) {
    // 当用户点击account表格中的项时会触发该信号，此槽函数用于处理信号
    if (item) {
        this->row_tableW = item->row();
        this->column_tableW = item->column();

        auto tableWidget = qobject_cast<QTableWidget*>(sender());
        if (tableWidget) {
            // ui->tableClickCoordViewer->setText(QString("r%1, c%2").arg(row_tableW).arg(column_tableW)); // 测试：显示坐标
            ui->coordViewer->setTextColor(QColor::fromRgbF(0, 255, 0, 1.0));
            ui->coordViewer->setText("已选中ID: " + tableWidget->item(row_tableW, 0)->text());
            // account表的显示控件：实时显示该行数据的id
        }
    }
}
