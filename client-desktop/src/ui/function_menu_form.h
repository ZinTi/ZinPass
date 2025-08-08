#ifndef FUNCTIONMENUFORM_H
#define FUNCTIONMENUFORM_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class FunctionMenuForm;
}

class FunctionMenuForm final : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionMenuForm(QWidget *parent = nullptr);
    ~FunctionMenuForm();

signals:
    // 自定义信号，携带按钮索引，用于通知主窗口或查看器显示内容
    void buttonClicked(int index);

private slots:
    void on_pBtnPwd_clicked();
    void on_pBtnTele_clicked();
    void on_pBtnCheck_clicked();
    void on_pBtnOwner_clicked();
    void on_pBtnOut_clicked();
    void on_pBtnIn_clicked();
    void on_pBtnLogLogin_clicked();
    void on_pBtnLogHandle_clicked();
    void on_pBtnEditPersonal_clicked();
    void on_pBtnCancelUser_clicked();
    void on_pBtnExit_clicked();

private:
    Ui::FunctionMenuForm *ui;
    QLabel* mascotLabel;

    void displayMascot(int ax, int ay, int maxHeight);
};

#endif // FUNCTIONMENUFORM_H
