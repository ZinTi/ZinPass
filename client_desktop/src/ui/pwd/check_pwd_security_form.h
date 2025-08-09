#ifndef CHECKPWDSECURITY_H
#define CHECKPWDSECURITY_H

#include <QWidget>

namespace Ui {
class CheckPwdSecurityForm;
}

class CheckPwdSecurityForm final : public QWidget
{
    Q_OBJECT

public:
    explicit CheckPwdSecurityForm(QWidget *parent = nullptr);
    ~CheckPwdSecurityForm();

private:
    Ui::CheckPwdSecurityForm *ui;
};

#endif