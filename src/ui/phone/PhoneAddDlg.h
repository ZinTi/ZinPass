#ifndef PHONEADDDLG_H
#define PHONEADDDLG_H

#include <QDialog>

namespace Ui {
class PhoneAddDlg;
}

class PhoneAddDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PhoneAddDlg(QWidget *parent = nullptr);
    ~PhoneAddDlg();

private slots:
    void on_pBtnClear_clicked(); // 清空输入
    void on_pBtnSubmit_clicked(); // 提交按钮

private:
    Ui::PhoneAddDlg *ui;
    void initialEditWidget();
};

#endif // PHONEADDDLG_H
