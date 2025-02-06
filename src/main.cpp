#include <QApplication>
#include "LoginDlg.h"
#include "Environ.h"
#include "SystemUser.h"
#include "MainWorkbench.h"

Environ* ZinPassEnv;
SystemUser* currentSystemUser;

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    ZinPassEnv = new Environ();
    currentSystemUser = new SystemUser();


    LoginDlg loginDlg(nullptr); // 实例化登录窗口

    if( loginDlg.exec() == QDialog::Accepted ){
        // 登录成功执行其他处理
        MainWorkbench mainWorkbench;
        // mainWorkbench.setStyleSheet("background-color: #8b008b;");
        mainWorkbench.show();
        return a.exec();
    }else{
        return 0;
    }
}

