#ifndef DIALOG_AUTH_H
#define DIALOG_AUTH_H

#include <QDialog>
#include <QLineEdit>

class DialogAuth final : public QDialog {
public:
    DialogAuth(QWidget* parent = nullptr);
    ~DialogAuth();

    QString get_input_main_key() const;

private slots:
    void on_btn_okay_clicked();

private:
    QLineEdit* edit_password_;
    QPushButton* btn_okay_;
    QPushButton* btn_cancel_;

    QString main_key_;
};

#endif // DIALOG_AUTH_H
