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
    void on_btn_begin_clicked() const; // 生成密码
    void on_btn_copy_clicked() const; // 复制结果
    void on_btn_clear_clicked() const; // 清空
    void on_btn_save_as_clicked(); // 另存为

    static void on_btn_cli_mode_clicked(); // 命令行模式

private:
    QLabel* l_title_; // 标题
    QTextEdit* txt_display_msg_; // 消息显示

    // 按钮
    QPushButton* btn_begin_; // 生成密码
    QPushButton* btn_copy_; // 复制结果
    QPushButton* btn_clear_; // 清空
    QPushButton* btn_save_as_; // 另存为
    QPushButton* btn_cli_mode_; // 命令行模式

    // 设置
    QPushButton* menu_components_Btn; // 创建一个按钮用于触发下拉菜单

    QCheckBox* chk_digits_;
    QCheckBox* chk_lowercase_;
    QCheckBox* chk_uppercase_;
    QCheckBox* chk_symbols_;

    QLabel* l_length_;
    QLabel* l_num_;
    QSpinBox* e_length_;
    QSpinBox* e_num_;
    QRadioButton* rdo_en_colorful_;

    // 显示
    QTextEdit* txt_display_pwd_;

    // 
    QMenu* menu_components_;
    QWidget* wgt_chk_;
    QWidgetAction* wgt_action_;

    // 布局
    QVBoxLayout* lyt_main_; // 主布局 从上至下三层
    QHBoxLayout* lyt_a1_;   // 1、header
    QHBoxLayout* lyt_a2_;   // 2、body
    QHBoxLayout* lyt_btn_;  // 3、footer

    QVBoxLayout* lyt_chk_box_;
    QVBoxLayout* lyt_setting_;
    QFormLayout* lyt_edit_;

};

#endif
