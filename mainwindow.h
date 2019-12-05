#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QValidator>
#include <QIntValidator>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>

#include "itemslist.h"
#include "evaluationmatrix.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui = nullptr;
    ItemsList* items_list_widget = nullptr;
    EvaluationMatrix* evaluation_matrix_widget = nullptr;
    QLineEdit* value_line_edit = nullptr, *weight_line_edit = nullptr;
    QLabel* consumed_weight_label = nullptr, *values_sum_label = nullptr, *used_items_label = nullptr;
    QSpinBox* max_weight_editor = nullptr;
    QPushButton* delete_item_button = nullptr;
    QCheckBox* auto_update_checkbox = nullptr;
    QSet<int> selected_items;
private slots:
    void addItem();
    void calculateKnapsack();
    void loadFile();
    void saveFile();
    void HandleItemsSelection(QVector<int> items);
    void deleteItems();
    void maximumWeightEdited(int max_weight);
public slots:
    void knapsackSolutionSelected(QString values_sum, QString consumed_weight, QVector<int> used_items);
};

#endif // MAINWINDOW_H
