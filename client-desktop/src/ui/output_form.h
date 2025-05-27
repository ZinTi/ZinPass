#ifndef OUTPUTFORM_H
#define OUTPUTFORM_H

#include <QWidget>
#include <QTextEdit>

class OutputForm : public QWidget
{
    Q_OBJECT

public:
    explicit OutputForm(QWidget *parent = nullptr);
    ~OutputForm();

private:
    void setup_ui();

    QTextEdit* edit_info_;
};

#endif // OUTPUTFORM_H
