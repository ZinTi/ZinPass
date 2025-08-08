#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>

QT_BEGIN_NAMESPACE

namespace Ui {
    class LoginDlg;
}

QT_END_NAMESPACE

class LoginDlg final : public QDialog {
    Q_OBJECT

public:
    LoginDlg(QWidget* parent = nullptr);
    ~LoginDlg();

private slots:
    void on_pBtnLogin_clicked(); // 处理登录按钮点击事件的槽函数
    void on_pBtnSignup_clicked(); // 处理注册按钮点击事件的槽函数

private:
    Ui::LoginDlg* ui;
    QLabel* logoLabel;

    void refreshComboBoxUserId() const;
    void displayLogo(int ax, int ay, int maxHeight);
};
#endif // LOGINDIALOG_H
