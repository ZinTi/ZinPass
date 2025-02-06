#include "PopupFilterWindow.h"
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QFormLayout>
#include "AccountService.h"

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
    QLabel* labelCategory = new QLabel("类别", this);
    QComboBox* comboBoxCategory = new QComboBox(this);
    // 加载所有类别
    AccountService accountService;
    std::vector<std::string> categories;
    std::string categoryMsg;
    accountService.getCategoryOptions(categoryMsg, categories);
    comboBoxCategory->clear(); // 清空原选项
    comboBoxCategory->addItem("所有");
    for (const std::string& option : categories) {
        comboBoxCategory->addItem(QString::fromStdString(option));
    }

    QLabel* labelUrl = new QLabel("网址", this);
    QLineEdit* editUrl = new QLineEdit();
    editUrl->setEnabled(false);

    QLabel* labelHotline = new QLabel("客服热线", this);
    QLineEdit* editHotline = new QLineEdit();
    editHotline->setEnabled(false);

    QLabel* labelSubAccount = new QLabel("子账号", this);
    QLineEdit* editSubAccount = new QLineEdit();
    editSubAccount->setEnabled(false);

    QLabel* labelPostscript= new QLabel("备注", this);
    QLineEdit* editPostscript = new QLineEdit();
    editPostscript->setEnabled(false);

    QLabel* labelCDTRange = new QLabel("创建时间范围", this);
    QDateTimeEdit* editCDTRangeStart = new QDateTimeEdit(this);
    editCDTRangeStart->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    editCDTRangeStart->setCalendarPopup(true);
    editCDTRangeStart->setEnabled(false);
    QDateTimeEdit* editCDTRangeEnd = new QDateTimeEdit(this);
    editCDTRangeEnd->setDisplayFormat(QString("yyyy-MM-dd HH:mm:ss"));
    editCDTRangeEnd->setCalendarPopup(true);
    editCDTRangeEnd->setEnabled(false);

    // 创建水平布局来放置两个日期输入框
    QVBoxLayout* dateRangeLayout = new QVBoxLayout;
    dateRangeLayout->addWidget(editCDTRangeStart);
    dateRangeLayout->addWidget(editCDTRangeEnd);

    // 使用 QFormLayout 布局筛选控件
    QFormLayout* formLayout = new QFormLayout();

    formLayout->addRow(labelCategory, comboBoxCategory);
    formLayout->addRow(labelUrl, editUrl);
    formLayout->addRow(labelHotline, editHotline);
    formLayout->addRow(labelSubAccount, editSubAccount);
    formLayout->addRow(labelPostscript, editPostscript);
    formLayout->addRow(labelCDTRange, dateRangeLayout);

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
