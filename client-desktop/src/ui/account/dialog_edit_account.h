#ifndef DIALOG_EDIT_ACCOUNT_H
#define DIALOG_EDIT_ACCOUNT_H

#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QGroupBox>
#include <QPushButton>

class DialogEditAccount : public QDialog {
    Q_OBJECT

public:
    explicit DialogEditAccount(const std::string& account_id, QWidget* parent = nullptr);
    ~DialogEditAccount();

    QMap<QString, QVariant> get_form_data() const;

signals:
    void form_submitted();

private slots:
    void on_btn_read_passwd_clicked();
    void on_btn_delete_clicked();
    void on_btn_submit_clicked();

    void update_edit_main_password_state();

private:
    std::string account_id_;

    void setup_ui();
    void initial_input_widgets();
    void read_only(bool enable = true);

    QGroupBox* group_box_platform_;
    QLineEdit* edit_provider_name_;
    QLineEdit* edit_platform_name_;
    QLineEdit* edit_url_;
    QComboBox* combo_category_;
    QLineEdit* edit_hotline_;

    QGroupBox* group_box_third_party_;
    QComboBox* combo_phone_;
    QComboBox* combo_email_;

    QGroupBox* group_box_account_;
    QLineEdit* edit_username_;
    QLineEdit* edit_nickname_;
    QLineEdit* edit_password_;
    QLineEdit* edit_sub_account_;

    QGroupBox* group_box_other_;
    QPlainTextEdit* edit_postscript_;

    QLineEdit* edit_main_password_;

    QPushButton* btn_refresh_;
    QPushButton* btn_read_passwd_;
    QPushButton* btn_edit_;
    QPushButton* btn_submit_;
    QPushButton* btn_delete_;
};

#endif // DIALOG_EDIT_ACCOUNT_H
