#ifndef INPUTFORM_H
#define INPUTFORM_H

#include <QWidget>
#include <QTextEdit>

class InputForm : public QWidget
{
    Q_OBJECT

public:
    explicit InputForm(QWidget *parent = nullptr);
    ~InputForm();

private:
    void setup_ui();

    QTextEdit* edit_info_;
};

#endif // INPUTFORM_H
