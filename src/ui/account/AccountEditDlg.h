#ifndef ACCOUNTEDITDLG_H
#define ACCOUNTEDITDLG_H

#include "ui_AccountDlg.h"
#include "Account.h"

class AccountEditDlg : public QDialog {
    Q_OBJECT

public:
    explicit AccountEditDlg(int accountId, QWidget *parent = nullptr);
    ~AccountEditDlg() override;
private slots:
    // 重置按钮的槽函数
    void on_pBtnReset_clicked() const;

    // 提交更新按钮的槽函数
    void on_pBtnSubmit_clicked();

private:
    Ui::AccountDlg* ui;
    QPushButton* pBtnReset;
    QPushButton* pBtnSubmit;

    int m_accountId; // 查询目标
    ViewAccount m_viewAccount;

    void initialData(); // 获取数据

};

#endif
