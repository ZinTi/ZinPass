#ifndef PHONEMGRFORM_H
#define PHONEMGRFORM_H

#include <QTableWidgetItem>
#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QToolButton>

#include "search_box.h"
#include "phone_filter_form.h"
#include "phone_detail_frame.h"

class PhoneMgrForm : public QWidget{
    Q_OBJECT

public:
    explicit PhoneMgrForm(QWidget *parent = nullptr);
    ~PhoneMgrForm();

private slots:
    void on_table_view_item_clicked(const QModelIndex &index);  // 点击表格项
    void on_btn_add_clicked();
    void on_btn_clear_clicked();

    void list_phones();

private:
    // 私有方法
    void setup_ui();

    // 控件 UI
    SearchBox* search_box_;         // 搜索控件
    QTextEdit* display_;            // 显示控件
    PhoneFilterForm* filter_form_;  // 筛选控件面板
    QStandardItemModel* table_model_;
    QTableView* table_view_;        // 表格

    QPushButton* btn_add_;
    QPushButton* btn_clear_;

    PhoneDetailFrame* phone_detail_frame_;

    // 数据成员变量
    int row_of_table_view_ = 0; // 聚焦行
    int column_of_table_view_ = 0;  // 聚焦列
};

#endif // PHONEMGRFORM_H
