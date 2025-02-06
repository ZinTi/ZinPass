#ifndef MAINWORKBENCH_H
#define MAINWORKBENCH_H

#include <QMainWindow>
#include <QStackedWidget>
#include "FunctionMenuForm.h" // 功能菜单
#include "AccountMgrForm.h" // 管理账号
#include "PhoneMgrForm.h" // 管理手机号
#include "CheckPwdSecurityForm.h" // 检查密码安全
#include "DataOwnerForm.h" // 管理数据所有权
#include "OutputForm.h"
#include "InputForm.h"
#include "PersonalMgrForm.h" // 管理个人账号
#include "CancelPersonalUserForm.h" // 注销账号

namespace Ui {
class MainWorkbench;
}

class MainWorkbench : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWorkbench(QWidget *parent = nullptr);
    ~MainWorkbench();

private slots:
    // 自定义槽函数，用于处理 m_functionMenuForm 的按钮点击事件
    void handleButtonClick(int index);

private:
    Ui::MainWorkbench *ui;
    FunctionMenuForm* m_functionMenuForm; // 功能菜单（左侧）
    QStackedWidget* m_stackedWidget; // 堆叠部件（右侧）

    QTabWidget* m_pageMgr; // 数据管理-选项卡部件
    QTabWidget* m_pageIO; // 迁移备份-选项卡部件
    QTabWidget* m_pageLog; // 日志信息-选项卡部件
    QTabWidget* m_pagePersonal; // 个人管理-选项卡部件

    void dataMgrPageInit();
    void ioMgrPageInit();
    void logMgrPageInit();
    void personalMgrPageInit();

    AccountMgrForm* m_accountMgrForm; // 管理账号
    PhoneMgrForm* m_phoneMgrForm; // 管理手机号
    CheckPwdSecurityForm* m_checkPwdSecurityForm; // 管理邮箱
    DataOwnerForm* m_dataOwnerForm; // 管理数据所有权

    OutputForm* m_outputForm; // 数据导出
    InputForm* m_inputForm; // 数据导入

    // LogForm_1* m_logForm1; // 登录日志
    // LogForm_2* m_logForm2; // 操作日志

    PersonalMgrForm* m_personalMgrForm; // 编辑个人资料
    CancelPersonalUserForm* m_cancelPersonalUserForm; // 注销账号

};

#endif
