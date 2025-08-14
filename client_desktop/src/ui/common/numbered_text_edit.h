/**
 * 带行号的文本编辑控件
 */
#ifndef NUMBERED_TEXT_EDIT_H
#define NUMBERED_TEXT_EDIT_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QObject>
#include <QTextBlock>

class line_number_area;

class NumberedTextEdit : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit NumberedTextEdit(QWidget* parent = nullptr);
    void line_number_area_paint_event(QPaintEvent* event);
    int line_number_area_width();

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void update_line_number_area_width(int new_block_count);
    void update_line_number_area(const QRect& rect, int dy);

private:
    QWidget* line_number_area;
    QFont line_number_font;
};

class line_number_area : public QWidget {
public:
    explicit line_number_area(NumberedTextEdit* editor) : QWidget(editor), text_edit(editor) {}
    QSize sizeHint() const override { return QSize(text_edit->line_number_area_width(), 0); }

protected:
    void paintEvent(QPaintEvent* event) override { text_edit->line_number_area_paint_event(event); }

private:
    NumberedTextEdit* text_edit;
};

#endif
