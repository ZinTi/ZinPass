#include "PwdGenDlg.h"
#include <QApplication>
#include <QDialog>
#include <QMenu>
#include <QPushButton>
#include <QWidgetAction>
#include <QFileDialog>
#include <QClipboard>
#include "PwdGenerator.h"

ComponentsMenuWidget::ComponentsMenuWidget(QWidget *parent) : QWidget(parent)
{
    m_pBtnComponentsMenu = new QPushButton("使用字符", this);

    // 创建菜单
    m_componentsMenu = new QMenu(this);

    // 创建一个包含四个QCheckBox的QWidget、创建四个QCheckBox并添加到布局中
    QWidget *checkBoxWidget = new QWidget();
    QVBoxLayout *vLayoutCheckBox = new QVBoxLayout(checkBoxWidget);

    m_checkBoxDigits.setText("数字（0-9）");
    m_checkBoxLowercase.setText("小写字母（a-z）");
    m_checkBoxUppercase.setText("大写字母（A-Z）");
    m_checkBoxSymbols.setText("其他字符（~!@#$等）");
    m_checkBoxDigits.setChecked(true);
    m_checkBoxLowercase.setChecked(true);
    m_checkBoxUppercase.setChecked(true);
    m_checkBoxSymbols.setChecked(true);

    vLayoutCheckBox->addWidget(&m_checkBoxDigits);
    vLayoutCheckBox->addWidget(&m_checkBoxLowercase);
    vLayoutCheckBox->addWidget(&m_checkBoxUppercase);
    vLayoutCheckBox->addWidget(&m_checkBoxSymbols);

    // 创建QWidgetAction并将包含QCheckBox的QWidget添加到其中
    QWidgetAction *widgetAction = new QWidgetAction(m_componentsMenu);
    widgetAction->setDefaultWidget(checkBoxWidget);

    // 将QWidgetAction添加到菜单中
    m_componentsMenu->addAction(widgetAction);

    // 为按钮绑定菜单
    m_pBtnComponentsMenu->setMenu(m_componentsMenu);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_pBtnComponentsMenu);
    setLayout(mainLayout);
}

ComponentsMenuWidget::~ComponentsMenuWidget(){
    // 析构函数中不需要手动删除子部件，Qt会自动处理
}

bool ComponentsMenuWidget::isDigitsChecked() const{
    return m_checkBoxDigits.isChecked();
}

bool ComponentsMenuWidget::isLowercaseChecked() const{
    return m_checkBoxLowercase.isChecked();
}

bool ComponentsMenuWidget::isUppercaseChecked() const{
    return m_checkBoxUppercase.isChecked();
}

bool ComponentsMenuWidget::isSymbolsChecked() const{
    return m_checkBoxSymbols.isChecked();
}

/******************************************************************************/

PwdGenDlg::PwdGenDlg(QWidget *parent) : QDialog(parent){
    initMember(); // 初始化控件
    initLayout(); // 初始化布局

    setWindowTitle(QString("随机密码生成器"));  // 设置对话框属性

    // 连接信号与槽
    connect(&m_pBtnBegin, &QPushButton::clicked, this, &PwdGenDlg::on_pBtnBegin_clicked);
    connect(&m_pBtnCopyResult, &QPushButton::clicked, this, &PwdGenDlg::on_pBtnCopyResult_clicked);
    connect(&m_pBtnClearDisplay, &QPushButton::clicked, this, &PwdGenDlg::on_pBtnClearDisplay_clicked);
    connect(&m_pBtnSaveAs, &QPushButton::clicked, this, &PwdGenDlg::on_pBtnSaveAs_clicked);
    connect(&m_pBtnCliMode, &QPushButton::clicked, this, &PwdGenDlg::on_pBtnCliMode_clicked);
}

void PwdGenDlg::initMember(){
    m_title.setText(QString("随机密码生成器"));
    m_title.setAlignment(Qt::AlignHCenter);

    m_msgDisplay.setText(QString("说明：<br/>1、长度范围6-99；<br/>2、数量范围1-99；"));
    m_msgDisplay.setReadOnly(true);
    m_msgDisplay.setFixedSize(150, 100);

    // 按钮
    m_pBtnBegin.setText(QString("生成密码")); // 生成密码
    m_pBtnCopyResult.setText(QString("复制结果")); // 复制结果
    m_pBtnClearDisplay.setText(QString("清空")); // 清空
    m_pBtnSaveAs.setText(QString("另存为")); // 另存为
    m_pBtnCliMode.setText(QString("命令行模式")); // 命令行模式

    m_componentsMenuWidget = new ComponentsMenuWidget(this); //

    m_labelLength.setText(QString("密码长度"));
    m_labelNum.setText(QString("生成数量"));
    m_labelColorful.setText(QString("使用颜色区分字符"));
    m_editLength.setRange(6, 999);
    m_editLength.setValue(12);
    m_editNum.setRange(1, 99);
    m_editNum.setValue(10);
    m_setColorful.setChecked(false);

    // 显示
    m_pwdDisplay.setPlaceholderText(QString("生成结果"));
    m_pwdDisplay.setReadOnly(true);
}

void PwdGenDlg::initLayout(){
    vLayoutMain.addLayout(&hLayoutHeader);
    vLayoutMain.addLayout(&hLayoutBody);
    vLayoutMain.addLayout(&hLayoutFooter);

    hLayoutHeader.addWidget(&m_title);

    vLayoutSetting.addWidget(&m_msgDisplay);
    vLayoutSetting.addWidget(m_componentsMenuWidget);
    vLayoutSetting.addLayout(&fLayoutEdit);

    fLayoutEdit.addRow(&m_labelLength, &m_editLength);
    fLayoutEdit.addRow(&m_labelNum, &m_editNum);
    fLayoutEdit.addRow(&m_setColorful, &m_labelColorful);

    hLayoutBody.addLayout(&vLayoutSetting);

    hLayoutBody.addWidget(&m_pwdDisplay);

    hLayoutFooter.addWidget(&m_pBtnBegin);
    hLayoutFooter.addWidget(&m_pBtnCopyResult);
    hLayoutFooter.addWidget(&m_pBtnClearDisplay);
    hLayoutFooter.addWidget(&m_pBtnSaveAs);
    hLayoutFooter.addWidget(&m_pBtnCliMode);

    setLayout(&vLayoutMain);
}

PwdGenDlg::~PwdGenDlg(){

}


void PwdGenDlg::on_pBtnBegin_clicked(){
    bool inDigits = m_componentsMenuWidget->isDigitsChecked();
    bool inLowercase = m_componentsMenuWidget->isLowercaseChecked();
    bool inUppercase = m_componentsMenuWidget->isUppercaseChecked();
    bool inSymbols = m_componentsMenuWidget->isSymbolsChecked();

    if(false == (inDigits || inLowercase || inUppercase || inSymbols)){
        m_msgDisplay.setTextColor(QColor::fromRgb(255, 0, 0));
        m_msgDisplay.setText(QString("至少选择一种字符"));
        return;
    }

    int inLength = m_editLength.value();
    int inNum = m_editNum.value();

    bool inColorful = m_setColorful.isChecked();

    std::vector<std::string> newPwds;

    PwdComponentsType components(inDigits, inUppercase, inLowercase, inSymbols);
    PwdGenerator pwdG(components, inLength, inNum);
    if(pwdG.getGwd(newPwds)){
        m_msgDisplay.setTextColor(QColor::fromRgb(0, 255, 0));
        m_msgDisplay.setText(QString("生成成功"));
        m_pwdDisplay.clear();
        if(inColorful){
            // 显示彩色字符：数字显示绿色、小写英文字母黄色、大写英文字母蓝色、其他字符紫色
            for (const std::string& pwd : newPwds) {
                QTextCursor cursor = m_pwdDisplay.textCursor();
                for (char c : pwd) {
                    QTextCharFormat format;
                    if (std::isdigit(c)) {
                        format.setForeground(QColor::fromRgb(50, 205, 50)); // 亮绿色
                    } else if (std::islower(c)) {
                        format.setForeground(QColor::fromRgb(255, 215, 0)); // 金黄色
                    } else if (std::isupper(c)) {
                        format.setForeground(QColor::fromRgb(0, 128, 255)); // 亮蓝色
                    } else {
                        format.setForeground(QColor::fromRgb(255, 105, 180)); // 粉红色
                    }
                    cursor.insertText(QString(c), format);
                }
                cursor.insertText("\n"); // 换行
            }
        }else{
            // 显示单色字符
            m_pwdDisplay.setTextColor(QColor::fromRgb(255, 100, 100));
            for(std::string pwd : newPwds){
                m_pwdDisplay.append(QString::fromStdString(pwd));
            }
        }
    }else{
        m_msgDisplay.setTextColor(QColor::fromRgb(255, 0, 0));
        m_msgDisplay.setText(QString("生成失败"));
    }
}
void PwdGenDlg::on_pBtnCopyResult_clicked()
{
    // 获取 QTextEdit 中的文本
    QString text = m_pwdDisplay.toPlainText();

    // 将 text 复制到剪贴板
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text);

    // 显示复制结果
    m_msgDisplay.setTextColor(QColor::fromRgb(0, 255, 0));
    m_msgDisplay.setText(QString("已经全部复制到剪贴板"));
}

void PwdGenDlg::on_pBtnClearDisplay_clicked(){
    m_pwdDisplay.clear();
}

void PwdGenDlg::on_pBtnSaveAs_clicked(){  // Save text to file.
    // 获取 QTextEdit 中的文本
    QString text = m_pwdDisplay.toPlainText();

    // 打开文件保存对话框
    QString filePath = QFileDialog::getSaveFileName(this, "另存为", "", "文本文件 (*.txt)");

    if (!filePath.isEmpty()) {
        // 打开文件进行写入操作
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // 使用 QTextStream 写入文本
            QTextStream out(&file);
            out << text;
            // 关闭文件
            file.close();
        }
    }
}

void PwdGenDlg::on_pBtnCliMode_clicked(){
#ifdef _WIN32
    system("cmd /C pwdgen.exe");
#else
    system("./pwdgen");
#endif
}
