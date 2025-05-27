#include "pwd_generator_dlg.h"
#include <QApplication>
#include <QDialog>
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidgetAction>
#include <QFormLayout>
#include <QFileDialog>
#include <QClipboard>
#include "pwd_generator.h"

PwdGeneratorDlg::PwdGeneratorDlg(QWidget *parent) : QDialog(parent){
    // 0\ 布局基本框架
    QVBoxLayout* vLayoutMain = new QVBoxLayout(this); // 主框架 从上至下三层
    QHBoxLayout* hLayoutA1 = new QHBoxLayout(); // 1、header
    QHBoxLayout* hLayoutA2 = new QHBoxLayout(); // 2、body
    QHBoxLayout* hLayoutBtn = new QHBoxLayout(); // 3、footer
    vLayoutMain->addLayout(hLayoutA1);
    vLayoutMain->addLayout(hLayoutA2);
    vLayoutMain->addLayout(hLayoutBtn);

    m_title = new QLabel(QString("随机密码生成器"));
    m_title->setAlignment(Qt::AlignHCenter);
    hLayoutA1->addWidget(m_title);

    // 1\ 编辑设置
    m_componentsMenuBtn = new QPushButton("使用字符", this);
    // 创建菜单
    QMenu *componentsMenu = new QMenu(this);

    // 创建一个包含四个QCheckBox的QWidget、创建四个QCheckBox并添加到布局中
    QWidget *checkBoxWidget = new QWidget();
    QVBoxLayout *vLayoutCheckBox = new QVBoxLayout(checkBoxWidget);

    m_checkBoxDigits = new QCheckBox("数字（0-9）", checkBoxWidget);
    m_checkBoxLowercase = new QCheckBox("小写字母（a-z）", checkBoxWidget);
    m_checkBoxUppercase = new QCheckBox("大写字母（A-Z）", checkBoxWidget);
    m_checkBoxSymbols = new QCheckBox("其他字符（~!@#$等）", checkBoxWidget);
    m_checkBoxDigits->setChecked(true);
    m_checkBoxLowercase->setChecked(true);
    m_checkBoxUppercase->setChecked(true);
    m_checkBoxSymbols->setChecked(true);

    vLayoutCheckBox->addWidget(m_checkBoxDigits);
    vLayoutCheckBox->addWidget(m_checkBoxLowercase);
    vLayoutCheckBox->addWidget(m_checkBoxUppercase);
    vLayoutCheckBox->addWidget(m_checkBoxSymbols);
    // 创建QWidgetAction并将包含QCheckBox的QWidget添加到其中
    QWidgetAction *widgetAction = new QWidgetAction(componentsMenu);
    widgetAction->setDefaultWidget(checkBoxWidget);

    // 将QWidgetAction添加到菜单中
    componentsMenu->addAction(widgetAction);

    // 为按钮绑定菜单
    m_componentsMenuBtn->setMenu(componentsMenu);

    QVBoxLayout* vLayoutSetting = new QVBoxLayout();
    m_msgDisplay = new QTextEdit(QString("说明：<br/>1、长度范围6-99；<br/>2、数量范围1-99；"));
    m_msgDisplay->setReadOnly(true);
    m_msgDisplay->setFixedSize(150, 100);
    vLayoutSetting->addWidget(m_msgDisplay);
    vLayoutSetting->addWidget(m_componentsMenuBtn);
    QFormLayout* fLayoutEdit = new QFormLayout();
    vLayoutSetting->addLayout(fLayoutEdit);
    m_labelLength = new QLabel(QString("密码长度"));
    m_labelNum = new QLabel(QString("生成数量"));
    m_labelColorful = new QLabel(QString("使用颜色区分字符"));
    m_editLength = new QSpinBox();
    m_editLength->setRange(6, 999);
    m_editLength->setValue(12);
    m_editNum = new QSpinBox();
    m_editNum->setRange(1, 99);
    m_editNum->setValue(10);
    m_setColorful = new QRadioButton();
    fLayoutEdit->addRow(m_labelLength, m_editLength);
    fLayoutEdit->addRow(m_labelNum, m_editNum);
    fLayoutEdit->addRow(m_setColorful, m_labelColorful);

    hLayoutA2->addLayout(vLayoutSetting);


    // 2\ 显示
    m_pwdDisplay = new QTextEdit();
    m_pwdDisplay->setReadOnly(true);
    m_pwdDisplay->setPlaceholderText(QString("生成结果"));
    hLayoutA2->addWidget(m_pwdDisplay);

    // 3\ 主要控制按钮

    m_pBtnBegin = new QPushButton(QString("生成密码"), this); // 生成密码
    m_pBtnCopyResult = new QPushButton(QString("复制结果"),this); // 复制结果
    m_pBtnClearDisplay = new QPushButton(QString("清空"), this); // 清空
    m_pBtnSaveAs = new QPushButton(QString("另存为"), this); // 另存为
    m_pBtnCliMode = new QPushButton(QString("命令行模式"), this); // 命令行模式

    hLayoutBtn->addWidget(m_pBtnBegin);
    hLayoutBtn->addWidget(m_pBtnCopyResult);
    hLayoutBtn->addWidget(m_pBtnClearDisplay);
    hLayoutBtn->addWidget(m_pBtnSaveAs);
    hLayoutBtn->addWidget(m_pBtnCliMode);

    // 4\ 整理对话框的布局


    // 5\ 设置对话框属性
    setWindowTitle(QString("随机密码生成器"));

    // 6\ 连接信号与槽
    connect(this->m_pBtnBegin, &QPushButton::clicked, this, &PwdGeneratorDlg::on_pBtnBegin_clicked);
    connect(this->m_pBtnCopyResult, &QPushButton::clicked, this, &PwdGeneratorDlg::on_pBtnCopyResult_clicked);
    connect(this->m_pBtnClearDisplay, &QPushButton::clicked, this, &PwdGeneratorDlg::on_pBtnClearDisplay_clicked);
    connect(this->m_pBtnSaveAs, &QPushButton::clicked, this, &PwdGeneratorDlg::on_pBtnSaveAs_clicked);
    connect(this->m_pBtnCliMode, &QPushButton::clicked, this, &PwdGeneratorDlg::on_pBtnCliMode_clicked);
}

PwdGeneratorDlg::~PwdGeneratorDlg(){
    if(m_title){
        delete m_title;
    }
    if(m_msgDisplay){
        delete m_msgDisplay;
    }

    if(m_pBtnBegin){
        delete m_pBtnBegin;
    }
    if(m_pBtnCopyResult){
        delete m_pBtnCopyResult;
    }
    if(m_pBtnClearDisplay){
        delete m_pBtnClearDisplay;
    }
    if(m_pBtnSaveAs){
        delete m_pBtnSaveAs;
    }
    if(m_pBtnCliMode){
        delete m_pBtnCliMode;
    }

    if(m_componentsMenuBtn){
        delete m_componentsMenuBtn;
    }

    if(m_checkBoxDigits){
        delete m_checkBoxDigits;
    }
    if(m_checkBoxLowercase){
        delete m_checkBoxLowercase;
    }
    if(m_checkBoxUppercase){
        delete m_checkBoxUppercase;
    }
    if(m_checkBoxSymbols){
        delete m_checkBoxSymbols;
    }

    if(m_labelLength){
        delete m_labelLength;
    }
    if(m_labelNum){
        delete m_labelNum;
    }
    if(m_labelColorful){
        delete m_labelColorful;
    }
    if(m_editLength){
        delete m_editLength;
    }
    if(m_editNum){
        delete m_editNum;
    }
    if(m_setColorful){
        delete m_setColorful;
    }

    if(m_pwdDisplay){
        delete m_pwdDisplay;
    }
}


void PwdGeneratorDlg::on_pBtnBegin_clicked(){
    bool inDigits = m_checkBoxDigits->isChecked();
    bool inLowercase = m_checkBoxLowercase->isChecked();
    bool inUppercase = m_checkBoxUppercase->isChecked();
    bool inSymbols = m_checkBoxSymbols->isChecked();

    if(false == (inDigits || inLowercase || inUppercase || inSymbols)){
        this->m_msgDisplay->setTextColor(QColor::fromRgb(255, 0, 0));
        this->m_msgDisplay->setText(QString("至少选择一种字符"));
        return;
    }

    int inLength = m_editLength->value();
    int inNum = m_editNum->value();

    bool inColorful = m_setColorful->isChecked();

    std::vector<std::string> newPwds;

    PwdComponentsType components(inDigits, inUppercase, inLowercase, inSymbols);
    PwdGenerator pwdG(components, inLength, inNum);
    if(pwdG.getGwd(newPwds)){
        this->m_msgDisplay->setTextColor(QColor::fromRgb(0, 255, 0));
        this->m_msgDisplay->setText(QString("生成成功"));
        m_pwdDisplay->clear();
        if(inColorful){
            // 显示彩色字符：数字显示绿色、小写英文字母黄色、大写英文字母蓝色、其他字符紫色
            for (const std::string& pwd : newPwds) {
                QTextCursor cursor = m_pwdDisplay->textCursor();
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
            this->m_pwdDisplay->setTextColor(QColor::fromRgb(255, 100, 100));
            for(std::string pwd : newPwds){
                this->m_pwdDisplay->append(QString::fromStdString(pwd));
            }
        }
    }else{
        this->m_msgDisplay->setTextColor(QColor::fromRgb(255, 0, 0));
        this->m_msgDisplay->setText(QString("生成失败"));
    }
}
void PwdGeneratorDlg::on_pBtnCopyResult_clicked()
{
    // 获取 QTextEdit 中的文本
    QString text = m_pwdDisplay->toPlainText();

    // 将 text 复制到剪贴板
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text);

    // 显示复制结果
    this->m_msgDisplay->setTextColor(QColor::fromRgb(0, 255, 0));
    this->m_msgDisplay->setText(QString("已经全部复制到剪贴板"));
}

void PwdGeneratorDlg::on_pBtnClearDisplay_clicked(){
    m_pwdDisplay->clear();
}

void PwdGeneratorDlg::on_pBtnSaveAs_clicked(){  // Save text to file.
    // 获取 QTextEdit 中的文本
    QString text = m_pwdDisplay->toPlainText();

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

void PwdGeneratorDlg::on_pBtnCliMode_clicked(){
#ifdef _WIN32
    system("cmd /C pwdgen.exe");
#else
    system("./pwdgen");
#endif
}
