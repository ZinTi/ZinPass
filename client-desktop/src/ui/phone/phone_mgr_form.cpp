#include "phone_mgr_form.h"
#include "mobile_phone.h"
#include <QMessageBox>
#include "widget_public_method.h"
#include "dialog_add_phone.h"
#include "state_manager.h"
#include "phone_rpc.h"

PhoneMgrForm::PhoneMgrForm(QWidget *parent) : QWidget(parent){
    setup_ui();


}

PhoneMgrForm::~PhoneMgrForm(){
}

void PhoneMgrForm::setup_ui(){
    // 创建控件
    this->search_box_ = new SearchBox(this);
    this->display_ = new QTextEdit(this);
    this->display_->setMaximumHeight(120);
    if(false == this->display_->isReadOnly()){
        this->display_->setReadOnly(true);
    }
    this->filter_form_ = new PhoneFilterForm();
    this->filter_form_->setMaximumHeight(120);

    this->table_view_ = new QTableView;
    this->table_view_->setSortingEnabled(true);
    this->table_model_ = new QStandardItemModel(0, 12, this);
    this->table_model_->setHorizontalHeaderLabels({"ID","手机号","运营商","服务密码","PIN","PUK","入网时间","归属地","备注","所属用户","创建时间","更新时间"});
    this->table_view_->setModel(this->table_model_);

    this->phone_detail_frame_ = new PhoneDetailFrame;
    this->phone_detail_frame_->setMaximumHeight(135);

    this->btn_add_ = new QPushButton("新增", this);
    this->btn_clear_ = new QPushButton("清屏", this);

    // 创建布局
    QVBoxLayout* lyt_main = new QVBoxLayout(this);
    QHBoxLayout* lyt_top = new QHBoxLayout;
    QVBoxLayout* lyt_btn = new QVBoxLayout;

    // 组合布局
    lyt_main->addWidget(this->search_box_);
    lyt_main->addLayout(lyt_top);
    lyt_main->addWidget(this->table_view_);
    lyt_main->addWidget(this->phone_detail_frame_);

    lyt_top->addWidget(this->display_);
    lyt_top->addLayout(lyt_btn);
    lyt_top->addWidget(this->filter_form_);

    lyt_btn->setAlignment(Qt::AlignCenter);
    lyt_btn->addWidget(this->btn_add_);
    lyt_btn->addWidget(this->btn_clear_);

    connect(this->search_box_, &SearchBox::search_triggered, this,
        [](const QString &keyword_){
            qDebug() << "用户在手机号管理界面点击了全局搜索操作\n";
        }
    );

    QObject::connect(this->filter_form_, &PhoneFilterForm::form_submitted,
        [&](){
            list_phones();
        }
    );

    connect(this->btn_add_, &QPushButton::clicked, this, &PhoneMgrForm::on_btn_add_clicked);
    connect(this->btn_clear_, &QPushButton::clicked, this, &PhoneMgrForm::on_btn_clear_clicked);
    connect(this->table_view_, &QTableView::clicked, this, &PhoneMgrForm::on_table_view_item_clicked); // 用户点击表格项，获取行列坐标保存到私有属性中，并显示Id列信息
}

void PhoneMgrForm::list_phones(){
    // 1. 准备数据
    auto data = this->filter_form_->get_form_data();
    const std::string session_id = zinpass::state::StateManager::instance().getUserState().session_id;

    std::string telecom_operator = (data["telecom"].toString().isEmpty() ? "" : data["telecom"].toString().toStdString());
    std::string phone_number = (data["phone"].toString().isEmpty() ? "" : data["phone"].toString().toStdString());


    // 2. 如果输入数据合法则查询数据，否则结束
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    zinpass::rpc::PhoneRPC phone_rpc(channel);

    const auto[phones, message] = phone_rpc.list_phones(session_id, telecom_operator, phone_number);
    unsigned int num_rows = static_cast<unsigned int>(phones.size());

    // 3. 渲染到 table view 中
    this->table_model_->setRowCount(num_rows);
    for(int i = 0 ; i < num_rows ; ++i){
        zinpass::models::MobilePhone row = phones[i];
        this->table_model_->setItem(i, 0, new QStandardItem(QString::number(row.getId())));
        this->table_model_->setItem(i, 1, new QStandardItem(QString::fromStdString(row.getPhoneNumber())));
        this->table_model_->setItem(i, 2, new QStandardItem(QString::fromStdString(row.getTelecomOperator())));
        this->table_model_->setItem(i, 3, new QStandardItem(QString::fromStdString(row.getServicePwd())));
        this->table_model_->setItem(i, 4, new QStandardItem(QString::fromStdString(row.getPin())));
        this->table_model_->setItem(i, 5, new QStandardItem(QString::fromStdString(row.getPuk())));
        this->table_model_->setItem(i, 6, new QStandardItem(QString::fromStdString(row.getJoinTime())));
        this->table_model_->setItem(i, 7, new QStandardItem(QString::fromStdString(row.getPhoneArea())));
        this->table_model_->setItem(i, 8, new QStandardItem(QString::fromStdString(row.getPostscript())));
        this->table_model_->setItem(i, 9, new QStandardItem(QString::fromStdString(row.getSysUserId())));
        this->table_model_->setItem(i, 10, new QStandardItem(QString::fromStdString(row.getCreatedTime())));
        this->table_model_->setItem(i, 11, new QStandardItem(QString::fromStdString(row.getUpdatedTime())));
    }

    // 4. 设置所有单元格为不可编辑
    for (int row = 0; row < this->table_model_->rowCount(); ++row) {
        for (int col = 0; col < this->table_model_->columnCount(); ++col) {
            QStandardItem *item = this->table_model_->item(row, col);
            if (item) {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable); // 移除可编辑标志
            }
        }
    }

    // 5、默认选中(0, 0)项
    if(0 >= this->table_model_->rowCount()){
        this->display_->setTextColor(QColor::fromRgbF(255, 100, 0, 1.0));
        this->display_->setText("没有找到符合的记录");
    }else{
        this->row_of_table_view_ = 0;
        this->column_of_table_view_ = 0;
        this->display_->setTextColor(QColor::fromRgbF(0, 100, 255, 1.0));
        QModelIndex index = this->table_model_->index(this->row_of_table_view_, 0);
        this->display_->setText(QString::fromStdString("查询到 %1 条手机号记录\n").arg(std::to_string(num_rows)));
        this->display_->append(QString("默认选中ID: %1").arg(this->table_model_->data(index).toString()));
    }
}

void PhoneMgrForm::on_btn_add_clicked(){
    DialogAddPhone dialog_add_phone(this);
    dialog_add_phone.exec();
}

void PhoneMgrForm::on_btn_clear_clicked() const {
    this->display_->clear();
}

void PhoneMgrForm::on_table_view_item_clicked(const QModelIndex &index) {
    if (!index.isValid()) return;

    this->row_of_table_view_ = index.row();
    this->column_of_table_view_ = index.column();

    QModelIndex index_of_first_column = index.sibling(this->row_of_table_view_, 0);  // 同一行，第0列
    QString value_of_first_column = index.model()->data(index_of_first_column).toString();


    this->display_->setTextColor(QColor::fromRgbF(0, 255, 0, 1.0));
    this->display_->append(
        QString("[操作] 查看ID为 %1 的手机号详细信息").arg(value_of_first_column)
    );
    this->phone_detail_frame_->fetch_phone_by_id(value_of_first_column.toInt());
    this->phone_detail_frame_->render_phone_to_ui();
    this->display_->append(
        QString("[本地] 已填充至下方控件框中")
    );
}
