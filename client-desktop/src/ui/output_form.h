#ifndef OUTPUT_FORM_H
#define OUTPUT_FORM_H

#include <QWidget>
#include <QTextEdit>

class OutputForm final : public QWidget
{
    Q_OBJECT

public:
    explicit OutputForm(QWidget *parent = nullptr);
    ~OutputForm();

private:
    void setup_ui();

    QTextEdit* edit_info_;
};

#endif
