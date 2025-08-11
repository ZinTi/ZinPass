#ifndef MAIN_WORKBENCH_H
#define MAIN_WORKBENCH_H

#include <QMainWindow>
#include <QStackedWidget>
#include "function_menu_form.h" // 功能菜单
#include "account_mgr_form.h" // 管理账号
#include "phone_mgr_form.h" // 管理手机号
#include "check_pwd_security_form.h" // 检查密码安全
#include "data_owner_form.h" // 管理数据归属
#include "output_form.h"
#include "input_form.h"
#include "log_run_form.h"
#include "log_handle_form.h"
#include "personal_mgr_form.h" // 管理个人账号
#include "preferences_form.h" // 注销账号

namespace Ui {
class MainWorkbench;
}

class MainWorkbench final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWorkbench(QWidget *parent = nullptr);
    ~MainWorkbench() override;

signals:
    void sig_btn_clicked(int index); // qt信号，请求 1-重新登录、2-直接退出

private slots:
    // 自定义槽函数，用于处理 func_menu_form_ 的按钮点击事件
    void handle_menu_select(int index);

private:
    Ui::MainWorkbench *ui;
    FunctionMenuForm* func_menu_form_; // 功能菜单（左侧）
    QStackedWidget* right_stacked_widget_; // 堆叠部件（右侧）

    QTabWidget* page_data_mgr_; // 数据管理-选项卡部件
    QTabWidget* page_transfer_; // 迁移备份-选项卡部件
    QTabWidget* page_log_; // 日志信息-选项卡部件
    QTabWidget* page_setting_; // 个人管理-选项卡部件

    void page_data_mgr_init();
    void page_transfer_init();
    void page_log_init();
    void page_setting_init();
    void close_this();

    AccountMgrForm* acc_mgr_form_; // 管理账号
    PhoneMgrForm* phone_mgr_form_; // 管理手机号
    CheckPwdSecurityForm* chk_pwd_security_form_; // 管理邮箱
    DataOwnerForm* data_owner_form_; // 管理数据归属

    OutputForm* output_form_; // 数据导出
    InputForm* input_form_; // 数据导入

    LogRunForm* log_run_form_; // 运行日志
    LogHandleForm* log_handle_form_; // 操作日志

    PersonalMgrForm* personal_mgr_form_; // 编辑个人资料
    PreferencesForm* preferences_form_; // 偏好设置
    QWidget* session_form_; // 会话设置

};

#endif
