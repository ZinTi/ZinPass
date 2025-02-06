#ifndef ACCOUNTMGRFORM_H
#define ACCOUNTMGRFORM_H

#include <QWidget>
#include <QTableWidget>
#include "PopupFilterWindow.h"

namespace Ui {
class AccountMgrForm;
}

class AccountMgrForm : public QWidget{
    Q_OBJECT

public:
    explicit AccountMgrForm(QWidget *parent = nullptr);
    ~AccountMgrForm();

private slots:
    void on_tBtnDropdownMore_clicked(); // 下拉筛选弹窗 按键


    void on_pBtnReset_clicked() const; // 清除筛选条件
    void on_pBtnQuery_clicked(); // 查询记录
    void on_pBtnShow_clicked(); // 查询和显示明文密码
    void on_pBtnAdd_clicked(); // 增加密码记录
    void on_pBtnUpdate_clicked(); // 更新记录
    void on_pBtnDelete_clicked(); // 删除密码记录

    void on_tBtnPwdGenerator_clicked(); // 随机密码生成器-toolBtn

    void on_tableWidgetAccount_itemClicked(const QTableWidgetItem *item); // 当用户点击表格中的项时会触发该信号，此槽函数用于处理信号


private:
    Ui::AccountMgrForm *ui;
    PopupFilterWindow *popupWindow; // 下拉筛选弹窗

    int row_tableW = 0; // 账号记录表中的鼠标聚焦行
    int column_tableW = 0; // 账号记录表的鼠标聚焦列

    void initialPopupFilterWindow(); // 为下拉筛选窗口初始化
    void initialAccountFilter() const; // 为account表的筛选器初始化数据

    // 创建下拉按钮，使用QToolButton
    QToolButton* tBtnDropdownMore;
};

#endif
