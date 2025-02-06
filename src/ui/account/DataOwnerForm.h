#ifndef DATAOWNERFORM_H
#define DATAOWNERFORM_H

#include <QWidget>

namespace Ui {
class DataOwnerForm;
}

class DataOwnerForm : public QWidget
{
    Q_OBJECT

public:
    explicit DataOwnerForm(QWidget *parent = nullptr);
    ~DataOwnerForm();

private:
    Ui::DataOwnerForm *ui;
};

#endif // DATAOWNERFORM_H
