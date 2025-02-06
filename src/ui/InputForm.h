#ifndef INPUTFORM_H
#define INPUTFORM_H

#include <QWidget>

namespace Ui {
class InputForm;
}

class InputForm : public QWidget
{
    Q_OBJECT

public:
    explicit InputForm(QWidget *parent = nullptr);
    ~InputForm();

private:
    Ui::InputForm *ui;
};

#endif // INPUTFORM_H
