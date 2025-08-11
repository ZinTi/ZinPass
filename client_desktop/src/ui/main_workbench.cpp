#include "main_workbench.h"
#include "ui_main_workbench.h"
#include <QHBoxLayout>
#include <QMessageBox>

MainWorkbench::MainWorkbench(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWorkbench) {
    ui->setupUi(this);

    func_menu_form_ = new FunctionMenuForm(this);
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* hLayout = new QHBoxLayout(centralWidget);
    hLayout->addWidget(func_menu_form_);

    right_stacked_widget_ = new QStackedWidget(this);  // 堆叠部件
    hLayout->addWidget(right_stacked_widget_);

    page_data_mgr_ = new QTabWidget(right_stacked_widget_); // 数据管理-选项卡部件
    page_transfer_ = new QTabWidget(right_stacked_widget_); // 迁移备份-选项卡部件
    page_log_ = new QTabWidget(right_stacked_widget_); // 日志信息-选项卡部件
    page_setting_ = new QTabWidget(right_stacked_widget_); // 个人管理-选项卡部件

    // 添加page
    right_stacked_widget_->addWidget(page_data_mgr_);
    right_stacked_widget_->addWidget(page_transfer_);
    right_stacked_widget_->addWidget(page_log_);
    right_stacked_widget_->addWidget(page_setting_);

    page_data_mgr_init(); // 添加几个tab
    page_transfer_init();
    page_log_init();
    page_setting_init();

    // 设置拉伸因子，第一个参数是要设置的索引，第二个参数是拉伸因子
    hLayout->setStretch(0, 1); // 给 func_menu_form_ 设置拉伸因子为 1
    hLayout->setStretch(1, 5); // 给 right_stacked_widget_ 设置拉伸因子为 5
    setCentralWidget(centralWidget);

    // 连接信号和槽
    connect(func_menu_form_, &FunctionMenuForm::sig_btn_clicked, this, &MainWorkbench::handle_menu_select);

}

MainWorkbench::~MainWorkbench() {    
    delete ui;
    delete func_menu_form_; // 功能菜单（左侧）
    delete right_stacked_widget_; // 堆叠部件（右侧）

    /* 这几个page添加到right_stacked_widget_以后由right_stacked_widget_统一管理和释放
    delete page_data_mgr_; // 数据管理-选项卡部件
    delete page_transfer_; // 迁移备份-选项卡部件
    delete page_log_; // 日志信息-选项卡部件
    delete page_setting_;
*/

    /* 这几个添加到page_data_mgr_以后由page_data_mgr_统一管理和释放
    delete acc_mgr_form_; // 管理账号
    delete phone_mgr_form_; // 管理手机号
    delete chk_pwd_security_form_; // 管理邮箱
    delete data_owner_form_; // 管理数据归属
*/


}

void MainWorkbench::handle_menu_select(const int index){
    // 设置当前显示的widget页面
    if(index>=1 && index<=4){
        right_stacked_widget_->setCurrentIndex(0);
    }else if(index>=5 && index<=6){
        right_stacked_widget_->setCurrentIndex(1);
    }else if(index>=7 && index<=8){
        right_stacked_widget_->setCurrentIndex(2);
    }else if(index>=9 && index<=12){
        right_stacked_widget_->setCurrentIndex(3);
    }else{
        QMessageBox::information(this, QString("无效的按键信号"), QString::fromStdString(std::to_string(index)));
    }
    // 设置当前tab
    switch(index){
    case 1:{
        page_data_mgr_->setCurrentIndex(0);
        break;
    }
    case 2:{
        page_data_mgr_->setCurrentIndex(1);
        break;
    }
    case 3:{
        page_data_mgr_->setCurrentIndex(2);
        break;
    }
    case 4:{
        page_data_mgr_->setCurrentIndex(3);
        break;
    }
    case 5:{
        page_transfer_->setCurrentIndex(0);
        break;
    }
    case 6:{
        page_transfer_->setCurrentIndex(1);
        break;
    }
    case 7:{
        page_log_->setCurrentIndex(0);
        break;
    }
    case 8:{
        page_log_->setCurrentIndex(1);
        break;
    }
    case 9:{
        page_setting_->setCurrentIndex(0);
        break;
    }
    case 10:{
        page_setting_->setCurrentIndex(1);
        break;
    }
    case 11: {
        page_setting_->setCurrentIndex(2);
        break;
    }
    case 12:{
        close_this();
        // break;
    }
    default:
        ;
    }
}

void MainWorkbench::page_data_mgr_init(){
    acc_mgr_form_ = new AccountMgrForm(page_data_mgr_); // 账号维护
    page_data_mgr_->addTab(acc_mgr_form_, QString("账号维护"));
    phone_mgr_form_ = new PhoneMgrForm(page_data_mgr_);
    page_data_mgr_->addTab(phone_mgr_form_, QString("手机号码"));
    chk_pwd_security_form_ = new CheckPwdSecurityForm(page_data_mgr_);
    page_data_mgr_->addTab(chk_pwd_security_form_, QString("安全检查"));
    data_owner_form_ = new DataOwnerForm(page_data_mgr_);
    page_data_mgr_->addTab(data_owner_form_, QString("数据归属"));
}
void MainWorkbench::page_transfer_init(){
    output_form_ = new OutputForm(page_transfer_); // 账号维护
    page_transfer_->addTab(output_form_, QString("数据导出"));
    input_form_ = new InputForm(page_transfer_);
    page_transfer_->addTab(input_form_, QString("数据导入"));
}
void MainWorkbench::page_log_init(){
    log_run_form_ = new LogRunForm(page_log_);
    page_log_->addTab(log_run_form_, QString("运行日志"));
    log_handle_form_ = new LogHandleForm(page_log_);
    page_log_->addTab(log_handle_form_, QString("操作日志"));
}
void MainWorkbench::page_setting_init(){
    personal_mgr_form_ = new PersonalMgrForm(page_setting_);
    page_setting_->addTab(personal_mgr_form_, QString("编辑个人资料"));
    preferences_form_ = new PreferencesForm(page_setting_);
    page_setting_->addTab(preferences_form_, QString("系统偏好设置"));
    session_form_ = new QWidget(page_setting_);
    page_setting_->addTab(session_form_, QString("会话设置"));
}
void MainWorkbench::close_this() {
    /*
    const QMessageBox::StandardButton reply = QMessageBox::question(this, QString("退出程序"), QString("确定退出吗？"), QMessageBox::Yes | QMessageBox::No); // 弹出确认退出的消息框
    if (QMessageBox::Yes == reply) { // 用户点击了“确定”，退出应用程序
        QApplication::quit(); // 直接退出程序
    }else if (QMessageBox::No == reply) {

    }*/

    QMessageBox question_box;
    question_box.setWindowTitle("退出系统");
    question_box.setText("是否重新登录？");

    const QPushButton* btn_re_login = question_box.addButton("重新登录", QMessageBox::AcceptRole); // 回车键默认触发
    const QPushButton* btn_quit = question_box.addButton("直接退出", QMessageBox::DestructiveRole); // 表示危险操作，可能高亮显示
    QPushButton* btn_cancel = question_box.addButton("取消", QMessageBox::ActionRole); // 普通动作按钮，无特殊行为
    question_box.exec(); // 自定义按钮需通过 clickedButton() 获取用户点击的按钮对象

    if (btn_re_login == question_box.clickedButton()) {
        emit sig_btn_clicked(1);
    } else if (btn_quit == question_box.clickedButton()) {
        emit sig_btn_clicked(2);
    }
    // 取消不做任何操作
}