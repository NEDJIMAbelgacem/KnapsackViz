#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    items_list_widget = ui->items_list_widget;
    evaluation_matrix_widget = ui->evaluation_matrix_widget;
    value_line_edit = ui->value_line_edit;
    weight_line_edit = ui->weight_line_edit;
    max_weight_editor = ui->max_weight_editor;
    values_sum_label = ui->values_sum_label;
    consumed_weight_label = ui->consumed_weight_label;
    used_items_label = ui->used_items_label;
    delete_item_button = ui->delete_item_btn;
    auto_update_checkbox = ui->auto_update_checkbox;

    ui->value_line_edit->setValidator(new QIntValidator(this));
    ui->weight_line_edit->setValidator(new QIntValidator(this));

    QObject::connect(ui->add_item_btn, SIGNAL(clicked()), this, SLOT(addItem()));
    QObject::connect(ui->calculate_btn, SIGNAL(clicked()), this, SLOT(calculateKnapsack()));

    QObject::connect(ui->load_file_action, SIGNAL(triggered(bool)), this, SLOT(loadFile()));
    QObject::connect(ui->save_file_action, SIGNAL(triggered(bool)), this, SLOT(saveFile()));

    QObject::connect(evaluation_matrix_widget, SIGNAL(knapsackSolutionSelectedSignal(QString, QString, QVector<int>)),
                     this, SLOT(knapsackSolutionSelected(QString, QString, QVector<int>)));
    QObject::connect(items_list_widget, SIGNAL(itemsSelectedSignal(QVector<int>)),
                     evaluation_matrix_widget, SLOT(ItemsSelected(QVector<int>)));
    QObject::connect(items_list_widget, SIGNAL(itemsSelectedSignal(QVector<int>)),
                     this, SLOT(HandleItemsSelection(QVector<int>)));

    QObject::connect(delete_item_button, SIGNAL(clicked()), this, SLOT(deleteItems()));
    QObject::connect(max_weight_editor, SIGNAL(valueChanged(int)), this, SLOT(maximumWeightEdited(int)));
}

void MainWindow::addItem() {
    int value = value_line_edit->text().toInt();
    int weight = weight_line_edit->text().toInt();
    items_list_widget->addItem(value, weight);
    evaluation_matrix_widget->addItem(value, weight);
    calculateKnapsack();
}

void MainWindow::calculateKnapsack() {
    QString w = max_weight_editor->text();
    evaluation_matrix_widget->calculateKnapsack(w.toInt());
}

void MainWindow::loadFile() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Text files (*.txt)"));
    QString file_path;
    bool error_happened = false;
    if (dialog.exec()) {
         file_path = dialog.selectedFiles()[0];
    } else {
        error_happened = true;
    }

    QFile file(file_path);
    if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        error_happened = true;
    }
    items_list_widget->clear();
    evaluation_matrix_widget->clear();
    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList lst = line.split(" ");
        if (lst.size() != 2) {
            error_happened = true;
            break;
        }
        int value = lst[0].toInt();
        int weight = lst[1].toInt();
        items_list_widget->addItem(value, weight);
        evaluation_matrix_widget->addItem(value, weight);
    }
    calculateKnapsack();
}

void MainWindow::saveFile() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Text files (*.txt)"));
    QString file_path;
    bool error_happened = false;
    if (dialog.exec()) {
         file_path = dialog.selectedFiles()[0];
    } else {
        error_happened = true;
    }

    QFile file(file_path);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate)) {
        error_happened = true;
    }
    if (error_happened) {
        qDebug() << "Error at " << __FILE__ << ":" << __LINE__;
        return;
    }
    QTextStream out(&file);
    for (QPair<int, int> p : items_list_widget->getItems()) {
        out << p.first << " " << p.second << "\n";
    }
    out.flush();
    file.close();
}

void MainWindow::knapsackSolutionSelected(QString values_sum, QString consumed_weight, QVector<int> used_items) {
    values_sum_label->setText(values_sum);
    consumed_weight_label->setText(consumed_weight);
    QString txt;
    for (int i : used_items) txt += QString("%1 ").arg(i);
    used_items_label->setText(txt);
    items_list_widget->itemsSelected(used_items);
}

void MainWindow::HandleItemsSelection(QVector<int> items) {
    selected_items.clear();
    for (auto i : items) selected_items.insert(i);
    delete_item_button->setEnabled(items.size() > 0);
}

void MainWindow::deleteItems() {
    items_list_widget->deleteItems(selected_items);
    evaluation_matrix_widget->deleteItems(selected_items);
    calculateKnapsack();
}

void MainWindow::maximumWeightEdited(int max_weight) {
    if (auto_update_checkbox->checkState() == Qt::CheckState::Checked) {
        calculateKnapsack();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
