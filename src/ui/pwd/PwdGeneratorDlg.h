#ifndef PWDGENERATORDLG_H
#define PWDGENERATORDLG_H

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QRadioButton>

class PwdGeneratorDlg : public QDialog{

public:
    explicit PwdGeneratorDlg(QWidget *parent = nullptr);
    ~PwdGeneratorDlg();

private slots:
    void on_pBtnBegin_clicked(); // 生成密码
    void on_pBtnCopyResult_clicked(); // 复制结果
    void on_pBtnClearDisplay_clicked(); // 清空
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

};

#endif // PWDGENERATORDLG_H
