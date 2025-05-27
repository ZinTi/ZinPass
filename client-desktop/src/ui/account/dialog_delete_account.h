#ifndef DIALOG_DELETE_ACCOUNT_H
#define DIALOG_DELETE_ACCOUNT_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <string>

class DialogDeleteAccount : public QDialog {
    Q_OBJECT

public:
    explicit DialogDeleteAccount(const long long account_id, QWidget* parent = nullptr);
    ~DialogDeleteAccount();

    void set_account_id(const long long account_id);
    std::string get_input_main_password() const;

private slots:
    void on_btn_okay_clicked();

private:
    QLineEdit* edit_password_;
    QPushButton* btn_okay_;
    QPushButton* btn_cancel_;

    long long account_id_;
    std::string main_password_;

    void setup_ui();
};

#endif // DIALOG_DELETE_ACCOUNT_H
