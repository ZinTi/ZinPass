#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include <QLineEdit>

class AuthDlg : public QDialog {
public:
    AuthDlg(QWidget* parent = nullptr);

    QString getInputMainPwd() const;

private slots:
    void on_pBtnOk_clicked();

private:
    QLineEdit* editPwd;
    QString m_mainPwd;
};

#endif // AUTHDIALOG_H
