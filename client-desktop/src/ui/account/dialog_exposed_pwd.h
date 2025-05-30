#ifndef DIALOG_EXPOSED_PWD_H
#define DIALOG_EXPOSED_PWD_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>

class DialogExposedPwd : public QDialog{
    Q_OBJECT

public:
    explicit DialogExposedPwd(long long account_id, QWidget *parent = nullptr);
    ~DialogExposedPwd() override;

private slots:
    void start_btn_countdown(QPushButton* btn);
    void update_button();

    void on_btn_read_clicked();             // 查看
    void on_btn_copy_clicked();             // 仅复制
    void on_btn_read_and_copy_clicked();    // 查看并复制

private:
    long long account_id_;

    void setup_ui();

    QTimer* timer_;     // 定时器
    int countdown_ = 0; // 倒计时秒数
    QString original_btn_text_;
    QPushButton* current_btn_;

    QTextEdit* edit_display_;
    QLineEdit* edit_main_password_;
    QPushButton* btn_read_;
    QPushButton* btn_copy_;
    QPushButton* btn_read_and_copy_;

};

#endif
