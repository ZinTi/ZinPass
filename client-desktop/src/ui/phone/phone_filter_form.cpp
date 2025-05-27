#include "phone_filter_form.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include "state_manager.h"
// RPC 封装
#include "telecom_rpc.h"
#include "phone_rpc.h"

PhoneFilterForm::PhoneFilterForm(QWidget *parent)
    : QWidget(parent){
    // setWindowTitle("筛选条件");
    setup_ui();
    initial_input_widgets(); // 初始化输入控件
}

PhoneFilterForm::~PhoneFilterForm(){
    if(this->combo_telecom_){ delete this->combo_telecom_ ; }
    if(this->combo_phone_){ delete this->combo_phone_ ; }

    if(this->btn_refresh_){ delete this->btn_refresh_ ; }
    if(this->btn_submit_){ delete this->btn_submit_ ; }
    if(this->box_filter_) { delete this->box_filter_; }
}


void PhoneFilterForm::setup_ui(){
    // 创建控件
    const int fixed_height = 24;    // 统一高度
    const int fixed_width = 56;
    this->box_filter_ = new QGroupBox("筛选条件", this);
    this->combo_telecom_ = new QComboBox(this);
    this->combo_telecom_->setFixedHeight(fixed_height);
    this->combo_phone_ = new QComboBox(this);
    this->combo_phone_->setFixedHeight(fixed_height);
    this->btn_refresh_ = new QPushButton("刷新", this);
    this->btn_submit_ = new QPushButton("查询", this);
    this->btn_refresh_->setFixedSize(QSize(fixed_width,fixed_height)); // 宽 高
    this->btn_submit_->setFixedSize(QSize(fixed_width,fixed_height));

    // 主布局
    QHBoxLayout* layout_main = new QHBoxLayout(this);
    QHBoxLayout* layout_groupbox = new QHBoxLayout(this);

    // 表单布局
    QFormLayout* layout_form_condition = new QFormLayout;
    layout_form_condition->addRow("运营商", this->combo_telecom_);
    layout_form_condition->addRow("手机号", this->combo_phone_);

    // 按钮布局
    QVBoxLayout* layout_btn = new QVBoxLayout;
    layout_btn->addWidget(this->btn_refresh_);
    layout_btn->addWidget(this->btn_submit_);
    layout_btn->addStretch(); // 拉伸

    // 表单对齐方式
    layout_form_condition->setFormAlignment(Qt::AlignTop | Qt::AlignLeft);

    // 组合布局
    layout_groupbox->addLayout(layout_form_condition);
    layout_groupbox->addSpacing(8);
    layout_groupbox->addLayout(layout_btn);
    // layout_groupbox->setContentsMargins(20, 20, 20, 20);
    this->box_filter_->setLayout(layout_groupbox);
    layout_main->addWidget(this->box_filter_);

    // 连接信号槽
    connect(this->btn_refresh_, &QPushButton::clicked, this, &PhoneFilterForm::on_btn_refresh_clicked);
    connect(this->btn_submit_, &QPushButton::clicked, this, &PhoneFilterForm::form_submitted);
}

void PhoneFilterForm::initial_input_widgets() const {
    // 1. 定义变量
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    // 2. 创建gRPC通道连接到服务器获取数据：可选 telecom_operators、phone_numbers
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::TelecomRPC telecom_rpc(channel);
    const auto[telecoms, message_telecom] = telecom_rpc.list_telecom_options(session_id);
    zinpass::rpc::PhoneRPC phone_rpc(channel);
    const auto[phone_numbers, message_phone] = phone_rpc.list_phone_numbers(session_id);

    // 3. 渲染到 UI 控件
    this->combo_telecom_->clear();
    this->combo_telecom_->addItem(QString("所有"));
    for (const std::string& telecom : telecoms) {
        this->combo_telecom_->addItem(QString::fromStdString(telecom));
    }

    this->combo_phone_->clear();
    this->combo_phone_->addItem(QString("所有")); // default
    for(const std::string& phone_number : phone_numbers){
        this->combo_phone_->addItem(QString::fromStdString(phone_number));
    }
}

void PhoneFilterForm::on_btn_refresh_clicked() {
    initial_input_widgets();     // 初始化控件
}

QMap<QString, QVariant> PhoneFilterForm::get_form_data() const{
    const QString telecom_operator = this->combo_telecom_->currentText();
    const QString phone_number = this->combo_phone_->currentText();
    return {
        {"telecom", (telecom_operator == "所有") ? "" : telecom_operator},
        {"phone", (phone_number == "所有") ? "" : phone_number}
    };
}
