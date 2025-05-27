#ifndef POPUPFILTERWINDOW_H
#define POPUPFILTERWINDOW_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QEvent>
#include <QMouseEvent>

// 自定义弹出窗口类
class PopupFilterWindow : public QWidget{
    Q_OBJECT
public:
    explicit PopupFilterWindow(QWidget *parent = nullptr);
    ~PopupFilterWindow() override;

protected:
    bool event(QEvent *event) override; // 重写鼠标按下事件，处理窗口外部点击

private:
    QLabel* m_title; // 标题
    QPushButton* m_pBtnReset; // 清空（重置）按钮
};
#endif // POPUPFILTERWINDOW_H
