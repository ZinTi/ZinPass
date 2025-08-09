#ifndef PWD_GEN_DLG_H
#define PWD_GEN_DLG_H

#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QMenu>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QWidgetAction>


class ComponentsMenuWidget final : public QWidget{
public:
    explicit ComponentsMenuWidget(QWidget *parent = nullptr);
    ~ComponentsMenuWidget() override;

    bool isDigitsChecked() const;
    bool isLowercaseChecked() const;
    bool isUppercaseChecked() const;
    bool isSymbolsChecked() const;

private:
    QPushButton* m_pBtnComponentsMenu;
    QMenu* m_componentsMenu;
    QCheckBox m_checkBoxDigits;
    QCheckBox m_checkBoxLowercase;
    QCheckBox m_checkBoxUppercase;
    QCheckBox m_checkBoxSymbols;

    QWidgetAction* widget_action_;
    QWidget* chk_box_widget_;
    
    QVBoxLayout* lyt_main_;
    QVBoxLayout* lyt_chk_box_;
};


class PwdGenDlg final : public QDialog{
public:
    explicit PwdGenDlg(QWidget *parent = nullptr);
    ~PwdGenDlg() override;

private slots:
    void on_pBtnBegin_clicked(); // 生成密码
    void on_pBtnCopyResult_clicked(); // 复制结果
    void on_pBtnClearDisplay_clicked(); // 清空
    void on_pBtnSaveAs_clicked(); // 另存为

    void on_pBtnCliMode_clicked(); // 命令行模式

private:
    QLabel m_title; // 标题
    QTextEdit m_msgDisplay; // 消息显示

    // 按钮
    QPushButton m_pBtnBegin; // 生成密码
    QPushButton m_pBtnCopyResult; // 复制结果
    QPushButton m_pBtnClearDisplay; // 清空
    QPushButton m_pBtnSaveAs; // 另存为
    QPushButton m_pBtnCliMode; // 命令行模式

    // 设置下拉菜单
    ComponentsMenuWidget *m_componentsMenuWidget;

    QLabel m_labelLength;
    QLabel m_labelNum;
    QLabel m_labelColorful;
    QSpinBox m_editLength;
    QSpinBox m_editNum;
    QRadioButton m_setColorful;

    // 显示
    QTextEdit m_pwdDisplay;

    // 布局
    QVBoxLayout vLayoutMain; // 主框架
    QHBoxLayout hLayoutHeader; // 1、header
    QHBoxLayout hLayoutBody; // 2、body
    QHBoxLayout hLayoutFooter; // 3、footer
    QVBoxLayout vLayoutSetting; // 2.1 设置
    QFormLayout fLayoutEdit; // 2.1.2

    void initMember();
    void initLayout();
};

#endif // PWD_GEN_DLG_H
