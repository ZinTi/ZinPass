#include "phone_detail_frame.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include "state_manager.h"
#include "phone_rpc.h"
#include "telecom_rpc.h"
#include "dialog_auth.h"

PhoneDetailFrame::PhoneDetailFrame(QFrame *parent) : QFrame(parent){
    this->phone_ = new zinpass::models::MobilePhone;
    setup_ui();
}

PhoneDetailFrame::~PhoneDetailFrame(){
    if(this->phone_){
        delete this->phone_;
    }
}

void PhoneDetailFrame::list_telecom_operators(){
    // 1. 定义变量
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    // 2. 创建gRPC通道连接到服务器获取 telecom_operators 数据
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::TelecomRPC telecom_rpc(channel);
    const auto[telecoms, message_telecom] = telecom_rpc.list_telecom_options(session_id);

    // 3. 渲染到 UI 控件
    this->combo_telecom_->clear();
    for (const std::string& telecom : telecoms) {
        this->combo_telecom_->addItem(QString::fromStdString(telecom));
    }
}

void PhoneDetailFrame::setup_ui(){
    this->edit_id_ = new QLineEdit(this);
    this->edit_id_->setEnabled(false);
    this->combo_telecom_ = new QComboBox(this);
    this->edit_phone_area_ = new QLineEdit(this);
    this->edit_phone_ = new QLineEdit(this);
    this->edit_pin_ = new QLineEdit(this);
    this->edit_puk_ = new QLineEdit(this);
    this->edit_service_pwd_ = new QLineEdit(this);
    this->edit_join_time_ = new QDateTimeEdit(this);
    this->edit_join_time_->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    this->edit_join_time_->setCalendarPopup(true); // 开启日历弹出功能
    this->edit_postscript_ = new QLineEdit(this);
    this->edit_user_id_ = new QLineEdit(this);
    this->edit_user_id_->setEnabled(false);
    this->edit_other_ = new QTextEdit(this);
    this->edit_other_->setPlaceholderText(
        "created_at: \n"
        "updated_at: \n");
    // this->edit_other_->setMaximumHeight(100);
    this->edit_other_->setReadOnly(true);

    this->btn_refresh_ = new QPushButton("刷新", this);
    this->btn_refresh_->setFixedWidth(60);
    this->btn_edit_ = new QPushButton("编辑", this);
    this->btn_edit_->setFixedWidth(60);
    this->btn_submit_ = new QPushButton("更新", this);
    this->btn_submit_->setFixedWidth(60);
    this->btn_delete_ = new QPushButton("删除", this);
    this->btn_delete_->setFixedWidth(60);

    QHBoxLayout* layout_main = new QHBoxLayout(this);
    QFormLayout* layout_form1 = new QFormLayout;
    QFormLayout* layout_form2 = new QFormLayout;
    QFormLayout* layout_form3 = new QFormLayout;
    QVBoxLayout* layout_btn = new QVBoxLayout;
    layout_main->addLayout(layout_form1);
    layout_main->addLayout(layout_form2);
    layout_main->addLayout(layout_form3);
    layout_main->addLayout(layout_btn);

    layout_form1->addRow("ID", this->edit_id_);
    layout_form1->addRow("运营商", this->combo_telecom_);
    layout_form1->addRow("归属地", this->edit_phone_area_);
    layout_form1->addRow("手机号", this->edit_phone_);

    layout_form2->addRow("PIN", this->edit_pin_);
    layout_form2->addRow("PUK", this->edit_puk_);
    layout_form2->addRow("服务密码", this->edit_service_pwd_);
    layout_form2->addRow("入网时间", this->edit_join_time_);

    layout_form3->addRow("备注", this->edit_postscript_);
    layout_form3->addRow("所有者", this->edit_user_id_);
    layout_form3->addRow("其他信息", this->edit_other_); // id + created_at + updated_at + postscript + owner_id

    layout_btn->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout_btn->addWidget(this->btn_refresh_);
    layout_btn->addWidget(this->btn_edit_);
    layout_btn->addWidget(this->btn_submit_);
    layout_btn->addWidget(this->btn_delete_);

    initial_input_widgets();

    connect(this->btn_refresh_, &QPushButton::clicked, this, &PhoneDetailFrame::initial_input_widgets);
    connect(this->btn_edit_, &QPushButton::clicked, this, [this, b = false](){
        PhoneDetailFrame::set_input_read_only(b);   // 允许编辑输入框
        this->btn_edit_->setEnabled(false);         // 禁用“编辑”按钮
        this->btn_edit_->setText("编辑中");          // 变更“编辑”按钮文字
        this->btn_submit_->setEnabled(true);        // 启用“更新”按钮
    });
    connect(this->btn_submit_, &QPushButton::clicked, this, &PhoneDetailFrame::on_btn_submit_clicked);
    connect(this->btn_delete_, &QPushButton::clicked, this, &PhoneDetailFrame::on_btn_delete_clicked);
}

void PhoneDetailFrame::initial_input_widgets(){
    this->edit_id_->clear();
    list_telecom_operators();   // 获取并列出电信运营商可选项
    this->edit_phone_area_->clear();
    this->edit_phone_->clear();
    this->edit_pin_->clear();
    this->edit_puk_->clear();
    this->edit_service_pwd_->clear();
    this->edit_join_time_->setDateTime(QDateTime::currentDateTime());
    this->edit_postscript_->clear();
    this->edit_other_->clear();

    set_input_read_only( true );            // 设置编辑框仅读
    this->btn_edit_->setEnabled( false );     // 默认“编辑” disable，获取数据后“编辑” enable
    this->btn_edit_->setText("编辑");
    this->btn_submit_->setEnabled( false );   // 默认“更新” disable，点击“编辑”后“更新” enable
    this->btn_delete_->setEnabled( false );   // 默认“删除” disable，填充数据后“删除” enable
}

void PhoneDetailFrame::set_input_read_only(bool enable){
    this->combo_telecom_->setEnabled( !enable );
    this->edit_phone_area_->setReadOnly(enable);
    this->edit_phone_->setReadOnly(enable);

    this->edit_pin_->setReadOnly(enable);
    this->edit_puk_->setReadOnly(enable);
    this->edit_service_pwd_->setReadOnly(enable);
    this->edit_join_time_->setReadOnly(enable);
    this->edit_postscript_->setReadOnly(enable);
}

void PhoneDetailFrame::fetch_phone_by_id(const int id){
    // 1. 准备数据
    this->phone_->setId(id);    // 先设置id
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    // 2. 查询数据
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::PhoneRPC phone_rpc(channel);
    const auto[phone, message] = phone_rpc.find_phone_by_id(session_id, this->phone_->getId());
    this->phone_->copy(phone);
}

void PhoneDetailFrame::render_phone_to_ui(){
    // enable "编辑"按钮 并禁用输入框编辑功能、暂时禁用编辑功能
    this->btn_edit_->setEnabled(true);
    this->btn_edit_->setText("编辑");
    set_input_read_only(true);
    if(this->btn_submit_->isEnabled()){
        this->btn_submit_->setEnabled(false);
    }
    this->btn_delete_->setEnabled(true);
    // 填充数据
    this->edit_id_->setText(QString("%1").arg(this->phone_->getId()));
    this->combo_telecom_->setCurrentText(QString::fromStdString(this->phone_->getTelecomOperator()));
    this->edit_phone_area_->setText(QString::fromStdString(this->phone_->getPhoneArea()));
    this->edit_phone_->setText(QString::fromStdString(this->phone_->getPhoneNumber()));
    this->edit_pin_->setText(QString::fromStdString(this->phone_->getPin()));
    this->edit_puk_->setText(QString::fromStdString(this->phone_->getPuk()));
    this->edit_service_pwd_->setText(QString::fromStdString(this->phone_->getServicePwd()));
    this->edit_postscript_->setText(QString::fromStdString(this->phone_->getPostscript()));
    this->edit_user_id_->setText(QString::fromStdString(this->phone_->getSysUserId()));

    QString other_info = QString(
        "[创建于] %1\n"
        "[更新于] %2\n"
        )
        .arg(this->phone_->getCreatedTime())
        .arg(this->phone_->getUpdatedTime());

    this->edit_other_->setText(other_info);

    if(this->phone_->getJoinTime().empty()){
        this->edit_join_time_->clear(); // 清除原值
        this->edit_join_time_->setDateTime(QDateTime());
        this->edit_join_time_->setSpecialValueText("空"); // 设置特殊值文本
        this->edit_join_time_->clearFocus(); // 让控件失去焦点，确保特殊值文本能正常显示
    }else{
        // 尝试将文本转换为 QDateTime 对象
        QDateTime join_time = QDateTime::fromString(
            QString::fromStdString(this->phone_->getJoinTime()),
            QString("yyyy-MM-dd HH:mm:ss"));
        if (join_time.isValid()) {  // 有效
            this->edit_join_time_->setDateTime(join_time);
        } else { // 转换失败
            this->edit_join_time_->clear();
            this->edit_join_time_->setDateTime(QDateTime());
            this->edit_join_time_->setSpecialValueText("无效值"); // 设置特殊值文本
            this->edit_join_time_->clearFocus(); // 让控件失去焦点，确保特殊值文本能正常显示
        }
    }
}

zinpass::models::MobilePhone PhoneDetailFrame::get_phone() const{
    return *this->phone_;
}

void PhoneDetailFrame::on_btn_submit_clicked(){
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    // 获取表单数据
    QString in_phone_id = this->edit_id_->text();
    if(in_phone_id.isEmpty()){
        QMessageBox::warning(this, QString("无效数据"), QString::fromStdString("请先查询并选中一条数据后再操作"));
        return;
    }

    QString in_telecom_operator = this->combo_telecom_->currentText();
    QString in_phone_area = this->edit_phone_area_->text();
    QString in_phone_number = this->edit_phone_->text();
    QString in_pin = this->edit_pin_->text();
    QString in_puk = this->edit_puk_->text();
    QString in_service_password = this->edit_service_pwd_->text();
    QString in_join_time = this->edit_join_time_->text();
    QString in_postscript = this->edit_postscript_->text();

    // 更新
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::PhoneRPC phone_rpc(channel);
    const auto[result, message] = phone_rpc.update_phone_by_id(
        session_id,
        in_phone_id.toInt(),
        in_telecom_operator.toStdString(),
        in_phone_area.toStdString(),
        in_phone_number.toStdString(),
        in_pin.toStdString(),
        in_puk.toStdString(),
        in_service_password.toStdString(),
        in_join_time.toStdString(),
        in_postscript.toStdString()
        );
    if(result){
        QMessageBox::information(this, QString("完成"), QString("更新成功! %1").arg(message));
    }else{
        QMessageBox::warning(this, QString("失败"), QString("更新失败! %1").arg(message));
    }
}

void PhoneDetailFrame::on_btn_delete_clicked(){
    // 1、准备数据
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;
    QString in_phone_id = this->edit_id_->text();
    if(in_phone_id.isEmpty()){
        QMessageBox::warning(this, QString("无效数据"), QString::fromStdString("请先查询并选中一条数据后再操作"));
        return;
    }

    const int dest_phone_id = in_phone_id.toInt();


    // 2. 鉴权
    /*
    AuthDlg authDlg;
    if (authDlg.exec() == QDialog::Accepted) {} else { return; } // 二次鉴权
    */

    // 3. 执行删除操作
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::PhoneRPC phone_rpc(channel);
    const auto[result, message] = phone_rpc.delete_phone_by_id(session_id, dest_phone_id);

    // 4. 更新UI：更新当前组件以及通知父组件更新 table_view
    if(result){
        QMessageBox::information(this, QString("完成"), QString("删除成功！%1").arg(message));
    }else{
        QMessageBox::warning(this, QString("失败"), QString("删除失败！%1").arg(message));
    }

    /*
    const int rowCount = ui->tableWidget->rowCount(); // 获取表格行数
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
    }*/
}


