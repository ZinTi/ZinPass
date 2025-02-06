#ifndef ACCOUNTADDDLG_H
#define ACCOUNTADDDLG_H

#include "ui_AccountDlg.h"

class AccountAddDlg : public QDialog {
    Q_OBJECT

public:
    explicit AccountAddDlg(QWidget *parent = nullptr);
    ~AccountAddDlg() override;

private slots:
    // 清空按钮的槽函数
    void on_pBtnClear_clicked() const;

    // 录入按钮的槽函数
    void on_pBtnSubmit_clicked();

private:
    Ui::AccountDlg *ui;
    QPushButton* pBtnClear;
    QPushButton* pBtnSubmit;

    void initialControl() const; // 初始化控件
};

#endif
