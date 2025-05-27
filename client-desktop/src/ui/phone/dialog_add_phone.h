#ifndef DIALOG_ADD_PHONE_H
#define DIALOG_ADD_PHONE_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QPushButton>

class DialogAddPhone : public QDialog {
    Q_OBJECT

public:
    explicit DialogAddPhone(QWidget* parent = nullptr);
    ~DialogAddPhone();

private slots:
    void on_btn_refresh_clicked();
    void on_btn_clear_clicked();
    void on_btn_submit_clicked();

private:
    // 控件
    QLineEdit* edit_phone_;
    QComboBox* combo_tele_operator_;
    QLineEdit* edit_service_password_;
    QLineEdit* edit_pin_;
    QLineEdit* edit_puk_;
    QDateTimeEdit* edit_join_time_;
    QLineEdit* edit_phone_area_;
    QTextEdit* edit_postscript_;
    QLineEdit* edit_main_password_;

    QPushButton* btn_refresh_;
    QPushButton* btn_clear_;
    QPushButton* btn_submit_;

    void setup_ui();
    void initial_edit_widget();

};

#endif // DIALOG_ADD_PHONE_H
