#ifndef CANCELPERSONALUSERFORM_H
#define CANCELPERSONALUSERFORM_H

#include <QWidget>

namespace Ui {
class CancelPersonalUserForm;
}

class CancelPersonalUserForm : public QWidget
{
    Q_OBJECT

public:
    explicit CancelPersonalUserForm(QWidget *parent = nullptr);
    ~CancelPersonalUserForm();

private:
    Ui::CancelPersonalUserForm *ui;
};

#endif // CANCELPERSONALUSERFORM_H
