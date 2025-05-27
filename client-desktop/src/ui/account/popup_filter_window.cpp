#include "popup_filter_window.h"
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QFormLayout>

PopupFilterWindow::PopupFilterWindow(QWidget* parent) : QWidget(parent, Qt::Popup) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    this->m_title = new QLabel(this);
    this->m_title->setText(QString("更多筛选条件"));
    this->m_title->setStyleSheet("color: yellow; font-size: 10pt;");
    this->m_title->setAlignment(Qt::AlignCenter);

    QHBoxLayout* hLayoutBtn = new QHBoxLayout;
    this->m_pBtnReset = new QPushButton("清空", this); // 清空筛选条件的Btn
    this->m_pBtnReset->setMaximumWidth(120); // 设置最大宽度
    hLayoutBtn->addWidget(this->m_pBtnReset, 0, Qt::AlignHCenter);

    mainLayout->addWidget(this->m_title); // 添加 title

    // 创建筛选控件
    QLabel* labelUrl = new QLabel("网址", this);
    QLineEdit* editUrl = new QLineEdit();
    editUrl->setEnabled(false);

    QLabel* labelHotline = new QLabel("客服热线", this);
    QLineEdit* editHotline = new QLineEdit();
    editHotline->setEnabled(false);

    QLabel* labelSubAccount = new QLabel("子账号", this);
    QLineEdit* editSubAccount = new QLineEdit();
    editSubAccount->setEnabled(false);

    QLabel* labelCDTRange = new QLabel("创建时间范围", this);
    QDateTimeEdit* editCDTRangeStart = new QDateTimeEdit(this);
    editCDTRangeStart->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    editCDTRangeStart->setCalendarPopup(true);
    editCDTRangeStart->setEnabled(false);
    QDateTimeEdit* editCDTRangeEnd = new QDateTimeEdit(this);
    editCDTRangeEnd->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    editCDTRangeEnd->setCalendarPopup(true);
    editCDTRangeEnd->setEnabled(false);

    QLabel* labelUDTRange = new QLabel("修改时间范围", this);
    QDateTimeEdit* editUDTRangeStart = new QDateTimeEdit(this);
    editUDTRangeStart->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    editUDTRangeStart->setCalendarPopup(true);
    editUDTRangeStart->setEnabled(false);
    QDateTimeEdit* editUDTRangeEnd = new QDateTimeEdit(this);
    editUDTRangeEnd->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    editUDTRangeEnd->setCalendarPopup(true);
    editUDTRangeEnd->setEnabled(false);
    editUDTRangeEnd->setDateTime(QDateTime::currentDateTime()); // 设置当前时间

    // 日期输入框布局
    QVBoxLayout* layoutCDTRange = new QVBoxLayout;
    layoutCDTRange->addWidget(editCDTRangeStart);
    layoutCDTRange->addWidget(editCDTRangeEnd);
    QVBoxLayout* layoutUDTRange = new QVBoxLayout;
    layoutUDTRange->addWidget(editUDTRangeStart);
    layoutUDTRange->addWidget(editUDTRangeEnd);

    // 使用 QFormLayout 布局筛选控件
    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow(labelUrl, editUrl);
    formLayout->addRow(labelHotline, editHotline);
    formLayout->addRow(labelSubAccount, editSubAccount);
    formLayout->addRow(labelCDTRange, layoutCDTRange);
    formLayout->addRow(labelUDTRange, layoutUDTRange);

    mainLayout->addLayout(formLayout); // 添加 formLayout
    mainLayout->addLayout(hLayoutBtn); // 添加包含 m_pBtnReset 的水平布局容器

    setLayout(mainLayout);
}

PopupFilterWindow::~PopupFilterWindow(){
    if(this->m_title){
        delete this->m_title;
    }
    if(this->m_pBtnReset){
        delete this->m_pBtnReset;
    }
}

bool PopupFilterWindow::event(QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (!geometry().contains(mouseEvent->globalPosition().toPoint())) {
            hide();
            return true;
        }
    }
    // 调用基类的事件处理方法，确保其他事件能正常处理
    return QWidget::event(event);
}
