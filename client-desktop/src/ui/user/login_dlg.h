#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>

class LoginDlg final : public QDialog {
    Q_OBJECT

public:
    LoginDlg(QWidget* parent = nullptr);
    ~LoginDlg() override;

private slots:
    void on_btn_login_clicked(); // 处理登录按钮点击事件的槽函数
    void on_btn_signup_clicked(); // 处理注册按钮点击事件的槽函数

private:
    QLabel* l_title_;
    QLabel* l_logo_;
    QPixmap* pixmap_logo_;

    QLabel* l_username_;
    QLabel* l_key_;

    QComboBox* combo_username_;
    QLineEdit* e_key_;

    QPushButton* btn_login_;
    QPushButton* btn_signup_;

    QLabel* l_copyleft_;

    QVBoxLayout* lyt_main_;
    QFormLayout* lyt_form_;
    QHBoxLayout* lyt_btn_;

    void setup_ui();
    void refreshComboBoxUserId() const;
};
#endif
