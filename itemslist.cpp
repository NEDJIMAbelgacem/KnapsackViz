#include "itemslist.h"
#include "ui_itemslist.h"

ItemsList::ItemsList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemsList)
{
    ui->setupUi(this);
}

ItemsList::~ItemsList()
{
    delete ui;
}
