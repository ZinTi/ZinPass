#include "WidgetPublicMethod.h"

void setColumnNonEditable(QTableWidget* tableWidget, const int column) {
    const int rowCount = tableWidget->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem* item = tableWidget->item(row, column);
        if (item == nullptr) {
            item = new QTableWidgetItem();
            tableWidget->setItem(row, column, item);
        }
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
}