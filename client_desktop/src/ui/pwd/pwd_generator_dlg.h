#ifndef PWD_GENERATOR_DLG_H
#define PWD_GENERATOR_DLG_H

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMenu>
#include <QWidgetAction>

class PwdGeneratorDlg final : public QDialog{

public:
    explicit PwdGeneratorDlg(QWidget *parent = nullptr);
    ~PwdGeneratorDlg() override;

private slots:
    void on_pBtnBegin_clicked() const; // 生成密码
    void on_pBtnCopyResult_clicked() const; // 复制结果
    void on_pBtnClearDisplay_clicked() const; // 清空
    void on_pBtnSaveAs_clicked(); // 另存为

    void on_pBtnCliMode_clicked(); // 命令行模式

private:
    QLabel* m_title; // 标题
    QTextEdit* m_msgDisplay; // 消息显示

    // 按钮
    QPushButton* m_pBtnBegin; // 生成密码
    QPushButton* m_pBtnCopyResult; // 复制结果
    QPushButton* m_pBtnClearDisplay; // 清空
    QPushButton* m_pBtnSaveAs; // 另存为
    QPushButton* m_pBtnCliMode; // 命令行模式

    // 设置
    QPushButton* m_componentsMenuBtn; // 创建一个按钮用于触发下拉菜单

    QCheckBox* m_checkBoxDigits;
    QCheckBox* m_checkBoxLowercase;
    QCheckBox* m_checkBoxUppercase;
    QCheckBox* m_checkBoxSymbols;

    QLabel* m_labelLength;
    QLabel* m_labelNum;
    QLabel* m_labelColorful;
    QSpinBox* m_editLength;
    QSpinBox* m_editNum;
    QRadioButton* m_setColorful;

    // 显示
    QTextEdit* m_pwdDisplay;

    // 
    QMenu* menu_components_;
    QWidget* checkBoxWidget_;
    QWidgetAction* widgetAction_;

    // 布局
    QVBoxLayout* lyt_main_; // 主布局 从上至下三层
    QHBoxLayout* lyt_a1_;   // 1、header
    QHBoxLayout* lyt_a2_;   // 2、body
    QHBoxLayout* lyt_btn_;  // 3、footer

    QVBoxLayout* lyt_chk_box_;
    QVBoxLayout* lyt_setting_;
    QFormLayout* lyt_edit_;

};

#endif // PWD_GENERATOR_DLG_H
