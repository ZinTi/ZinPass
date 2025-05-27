#ifndef ACCOUNTMGRFORM_H
#define ACCOUNTMGRFORM_H

#include <QWidget>
#include <QTableWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QToolButton>

#include "search_box.h"
#include "account_filter_form.h"
#include "pwd_gen_dlg.h"
#include "dialog_add_account.h"
#include "dialog_edit_account.h"
// #include "popup_filter_window.h"

class AccountMgrForm : public QWidget{
    Q_OBJECT

public:
    explicit AccountMgrForm(QWidget *parent = nullptr);
    ~AccountMgrForm();

private slots:
    void on_btn_read_passwd_clicked(); // 查询和显示明文密码
    void on_btn_add_account_clicked(); // 增加密码记录
    void on_btn_detail_and_edit_clicked(); // 更新记录
    void on_btn_remove_account_clicked(); // 删除密码记录
    void on_btn_passwd_generator_clicked(); // 随机密码生成器-toolBtn
    void on_table_view_item_clicked(const QModelIndex &index);

    void list_accounts();    // 查询accounts

private:
    void setup_ui();

    SearchBox* search_box_;         // 搜索控件
    QTextEdit* coord_viewer_;       // 显示控件
    AccountFilterForm* filter_form_; // 筛选控件面板
    QStandardItemModel* table_model_;
    QTableView* table_view_;        // 表格
    PwdGenDlg* passwd_generator_;   // 密码生成器
    DialogAddAccount* dialog_add_account_; // 新增账号
    DialogEditAccount* dialog_detail_and_edit_;   // 详情与编辑

    QToolButton* btn_data_owner_;   // 数据所有权
    QToolButton* btn_passwd_generator_; // 随机密码生成器
    QPushButton* btn_read_passwd_;  // 查看密码
    QPushButton* btn_add_account_;  // 新增
    QPushButton* btn_detail_and_edit_;  // 更新

    int row_of_table_view_ = 0; // 聚焦行
    int column_of_table_view_ = 0;  // 聚焦列
};

#endif
