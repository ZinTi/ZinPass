#ifndef PERSONALMGRFORM_H
#define PERSONALMGRFORM_H

#include <QWidget>

namespace Ui {
class PersonalMgrForm;
}

class PersonalMgrForm : public QWidget
{
    Q_OBJECT

public:
    explicit PersonalMgrForm(QWidget *parent = nullptr);
    ~PersonalMgrForm();

private:
    Ui::PersonalMgrForm *ui;
};

#endif // PERSONALMGRFORM_H
