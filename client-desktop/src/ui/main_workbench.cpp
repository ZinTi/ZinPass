#include "main_workbench.h"
#include "ui_main_workbench.h"
#include <QHBoxLayout>
#include <QMessageBox>

MainWorkbench::MainWorkbench(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWorkbench) {
    ui->setupUi(this);

    m_functionMenuForm = new FunctionMenuForm(this);
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* hLayout = new QHBoxLayout(centralWidget);
    hLayout->addWidget(m_functionMenuForm);

    m_stackedWidget = new QStackedWidget(this);  // 堆叠部件
    hLayout->addWidget(m_stackedWidget);

    m_pageMgr = new QTabWidget(m_stackedWidget); // 数据管理-选项卡部件
    m_pageIO = new QTabWidget(m_stackedWidget); // 迁移备份-选项卡部件
    m_pageLog = new QTabWidget(m_stackedWidget); // 日志信息-选项卡部件
    m_pagePersonal = new QTabWidget(m_stackedWidget); // 个人管理-选项卡部件

    // 添加几个page
    m_stackedWidget->addWidget(m_pageMgr);
    m_stackedWidget->addWidget(m_pageIO);
    m_stackedWidget->addWidget(m_pageLog);
    m_stackedWidget->addWidget(m_pagePersonal);

    dataMgrPageInit(); // 添加几个tab
    ioMgrPageInit();
    logMgrPageInit();
    personalMgrPageInit();

    // 设置拉伸因子，第一个参数是要设置的索引，第二个参数是拉伸因子
    hLayout->setStretch(0, 1); // 给 m_functionMenuForm 设置拉伸因子为 1
    hLayout->setStretch(1, 5); // 给 m_stackedWidget 设置拉伸因子为 5
    setCentralWidget(centralWidget);

    // 连接信号和槽
    connect(m_functionMenuForm, &FunctionMenuForm::buttonClicked, this, &MainWorkbench::handleButtonClick);

}

MainWorkbench::~MainWorkbench() {    
    delete ui;
    delete m_functionMenuForm; // 功能菜单（左侧）
    delete m_stackedWidget; // 堆叠部件（右侧）

    /* 这几个page添加到m_stackedWidget以后由m_stackedWidget统一管理和释放
    delete m_pageMgr; // 数据管理-选项卡部件
    delete m_pageIO; // 迁移备份-选项卡部件
    delete m_pageLog; // 日志信息-选项卡部件
    delete m_pagePersonal;
*/

    /* 这几个添加到m_pageMgr以后由m_pageMgr统一管理和释放
    delete m_accountMgrForm; // 管理账号
    delete m_phoneMgrForm; // 管理手机号
    delete m_checkPwdSecurityForm; // 管理邮箱
    delete m_dataOwnerForm; // 管理数据所有权
*/


}

void MainWorkbench::handleButtonClick(const int index){
    // 设置当前显示的widget页面
    if(index>=1 && index<=4){
        m_stackedWidget->setCurrentIndex(0);
    }else if(index>=5 && index<=6){
        m_stackedWidget->setCurrentIndex(1);
    }else if(index>=7 && index<=8){
        m_stackedWidget->setCurrentIndex(2);
    }else if(index>=9 && index<=12){
        m_stackedWidget->setCurrentIndex(3);
    }else{
        QMessageBox::information(this, QString("无效的按键信号"), QString::fromStdString(std::to_string(index)));
    }
    // 设置当前tab
    switch(index){
    case 1:{
        m_pageMgr->setCurrentIndex(0);
        break;
    }
    case 2:{
        m_pageMgr->setCurrentIndex(1);
        break;
    }
    case 3:{
        m_pageMgr->setCurrentIndex(2);
        break;
    }
    case 4:{
        m_pageMgr->setCurrentIndex(3);
        break;
    }
    case 5:{
        m_pageIO->setCurrentIndex(0);
        break;
    }
    case 6:{
        m_pageIO->setCurrentIndex(1);
        break;
    }
    case 7:{
        m_pageLog->setCurrentIndex(0);
        break;
    }
    case 8:{
        m_pageLog->setCurrentIndex(1);
        break;
    }
    case 9:{
        m_pagePersonal->setCurrentIndex(0);
        break;
    }
    case 10:{
        m_pagePersonal->setCurrentIndex(1);
        break;
    }
    case 11: {
        m_pagePersonal->setCurrentIndex(2);
        break;
    }
    case 12:{
        // 弹出确认退出的消息框
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, QString("退出程序"), QString("确定退出吗？"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) { // 用户点击了“确定”，退出应用程序
            QApplication::quit();
        }
        // break;
    }
    default:
        ;
    }
}

void MainWorkbench::dataMgrPageInit(){
    m_accountMgrForm = new AccountMgrForm(m_pageMgr); // 账号管理
    m_pageMgr->addTab(m_accountMgrForm, QString("账号管理"));
    m_phoneMgrForm = new PhoneMgrForm(m_pageMgr);
    m_pageMgr->addTab(m_phoneMgrForm, QString("手机号管理"));
    m_checkPwdSecurityForm = new CheckPwdSecurityForm(m_pageMgr);
    m_pageMgr->addTab(m_checkPwdSecurityForm, QString("安全检查"));
    m_dataOwnerForm = new DataOwnerForm(m_pageMgr);
    m_pageMgr->addTab(m_dataOwnerForm, QString("数据所有权"));
}
void MainWorkbench::ioMgrPageInit(){
    m_outputForm = new OutputForm(m_pageIO); // 账号管理
    m_pageIO->addTab(m_outputForm, QString("数据导出"));
    m_inputForm = new InputForm(m_pageIO);
    m_pageIO->addTab(m_inputForm, QString("数据导入"));
}
void MainWorkbench::logMgrPageInit(){
    log_run_form_ = new LogRunForm(m_pageLog);
    m_pageLog->addTab(log_run_form_, QString("运行日志"));
    log_handle_form_ = new LogHandleForm(m_pageLog);
    m_pageLog->addTab(log_handle_form_, QString("操作日志"));
}
void MainWorkbench::personalMgrPageInit(){
    m_personalMgrForm = new PersonalMgrForm(m_pagePersonal);
    m_pagePersonal->addTab(m_personalMgrForm, QString("编辑个人资料"));
    m_cancelPersonalUserForm = new PreferencesForm(m_pagePersonal);
    m_pagePersonal->addTab(m_cancelPersonalUserForm, QString("系统偏好设置"));
    QWidget* sessionWidget = new QWidget(m_pagePersonal);
    m_pagePersonal->addTab(sessionWidget, QString("会话设置"));
}
