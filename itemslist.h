#ifndef ITEMSLIST_H
#define ITEMSLIST_H

#include <QWidget>

namespace Ui {
class ItemsList;
}

class ItemsList : public QWidget
{
    Q_OBJECT

public:
    explicit ItemsList(QWidget *parent = 0);
    ~ItemsList();

private:
    Ui::ItemsList *ui;
};

#endif // ITEMSLIST_H
