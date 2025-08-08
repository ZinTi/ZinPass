#ifndef CANCEL_PERSONAL_USER_FORM_H
#define CANCEL_PERSONAL_USER_FORM_H

#include <QWidget>

class CancelPersonalUserForm final : public QWidget
{
    Q_OBJECT

public:
    explicit CancelPersonalUserForm(QWidget *parent = nullptr);
    ~CancelPersonalUserForm();

private:
};

#endif // CANCEL_PERSONAL_USER_FORM_H
