#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>

namespace Ui {
class SignupDlg;
}

class SignupDlg final : public QDialog{
    Q_OBJECT

public:
    explicit SignupDlg(QWidget *parent = nullptr);
    ~SignupDlg() override;

private slots:
    void on_btn_submit_clicked(); // 处理注册按钮点击事件的槽函数
    void on_btn_clear_clicked() const; // 处理清空按钮点击事件的槽函数

private:
    Ui::SignupDlg *ui;

};

#endif // SIGNUPDIALOG_H
