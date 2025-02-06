#ifndef EXPOSEDPWDDIALOG_H
#define EXPOSEDPWDDIALOG_H

#include <QDialog>

namespace Ui {
class ExposedPwdDlg;
}

class ExposedPwdDlg : public QDialog{
    Q_OBJECT

public:
    explicit ExposedPwdDlg(int accountId, QWidget *parent = nullptr);
    ~ExposedPwdDlg() override;

private slots:
    void on_pBtnSubmit_clicked(); // 提交密码验证身份

private:
    Ui::ExposedPwdDlg *ui;
    int accountId;
};

#endif
