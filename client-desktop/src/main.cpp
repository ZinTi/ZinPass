#include <QApplication>
#include <QDir>
#include "state_manager.h"
#include "login_dlg.h"
#include "main_workbench.h"

using namespace zinpass;

void init();    // 客户端初始化

int main(int argc, char *argv[]){
    init();

    QApplication app(argc, argv);
    LoginDlg login_dlg(nullptr); // 实例化登录窗口
    if (login_dlg.exec() == QDialog::Accepted) {
        // 登录成功执行其他处理
        MainWorkbench mainWorkbench;
        mainWorkbench.setWindowTitle("工作台 ZinPass v0.0.4");
        // mainWorkbench.setStyleSheet("background-color: #8b008b;");
        mainWorkbench.show();
        return app.exec();
    }

    return 0;
}

void init() {
    // 检测客户端根路径（依据当前程序所在目录的父目录）
    const QString executablePath = QCoreApplication::applicationDirPath();
    if (QDir dir(executablePath); dir.cdUp()) {
        const std::string clientRootPath = dir.absolutePath().toStdString();
        auto& state = state::StateManager::instance();
        state.setClientRootPath(clientRootPath);    // 设置当前客户端路径
    } else {
        // "Failed to get parent directory";
    }
}
