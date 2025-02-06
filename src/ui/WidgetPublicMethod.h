#ifndef WIDGETPUBLICMETHOD_H
#define WIDGETPUBLICMETHOD_H

#include <QTableWidget>


/**
 * @brief 该函数接收一个 QTableWidget 指针和一个要设置为不可编辑的列索引
 * @param tableWidget 要设置的表格指针
 * @param column 要设置的列
 */
void setColumnNonEditable(QTableWidget* tableWidget, const int column);

#endif
