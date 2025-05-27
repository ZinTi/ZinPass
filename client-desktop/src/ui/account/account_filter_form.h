#ifndef ACCOUNT_FILTER_FORM_H
#define ACCOUNT_FILTER_FORM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QToolButton>
#include <QGroupBox>
#include "popup_filter_window.h"

class AccountFilterForm : public QWidget{
    Q_OBJECT

public:
    explicit AccountFilterForm(QWidget *parent = nullptr);
    ~AccountFilterForm();

    QMap<QString, QVariant> get_form_data() const;

signals:
    void form_submitted();

private slots:
    void on_btn_refresh_clicked();          // 处理刷新按键点击事件
    void on_btn_drop_down_more_clicked();   // 处理下拉按键点击事件

private:
    void setup_ui();
    void setup_layout();
    void initial_popup_window();        // 初始化下拉筛选窗口
    void initial_input_widgets() const;  // 初始化输入控件

    // 1. 基本控件
    QLineEdit* edit_provider_name_;
    QLineEdit* edit_platform_name_;
    QLineEdit* edit_username_;
    QLineEdit* edit_nickname_;
    QComboBox* combo_phone_;
    QComboBox* combo_email_;
    QComboBox* combo_category_;
    QLineEdit* edit_postscript_;
    QPushButton* btn_refresh_;
    QPushButton* btn_submit_;
    QGroupBox* box_filter_;

    // 2. 基本控件 + 自定义控件
    QToolButton* btn_drop_down_more_;    // 下拉按钮
    PopupFilterWindow* popup_window_;    // 下拉弹窗
};

#endif // ACCOUNT_FILTER_FORM_H
