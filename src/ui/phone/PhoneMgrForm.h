#ifndef PHONEMGRFORM_H
#define PHONEMGRFORM_H

#include <QTableWidgetItem>
#include <QWidget>

namespace Ui {
class PhoneMgrForm;
}

class PhoneMgrForm : public QWidget{
    Q_OBJECT

public:
    explicit PhoneMgrForm(QWidget *parent = nullptr);
    ~PhoneMgrForm();

private slots:
    void on_pBtnClear_clicked();
    void on_pBtnQuery_clicked();
    void on_pBtnRefresh_clicked();
    void on_pBtnAdd_clicked();
    void on_pBtnUpdate_clicked();
    void on_pBtnDelete_clicked();

    void on_tableWidget_itemClicked(const QTableWidgetItem *item); // 当用户点击表格中的项时触发信号

private:
    Ui::PhoneMgrForm *ui;

    int row_tableW = 0; // 手机号记录表中的鼠标聚焦行
    int column_tableW = 0; // 手机号记录表中的鼠标聚焦列
    std::vector<std::string> m_teleOperators; // 手机运营商可选项
    std::vector<std::string> m_phoneNumbers; // 手机号可选项

    bool getAllTeleOperatorOptions();
    bool getAllMobilePhoneOptions();
    void initialPhoneFilterAndEditor();
};

#endif // PHONEMGRFORM_H
