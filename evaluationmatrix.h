#ifndef EVALUATIONMATRIX_H
#define EVALUATIONMATRIX_H

#include <QWidget>
#include <QAbstractTableModel>
#include <QTableView>
#include <QVariant>
#include <iostream>
#include <QFile>
#include <QItemSelectionModel>
#include <QPair>
#include <QMap>

#define MAX_INT 1000000

namespace Ui {
class EvaluationMatrix;
}

class EvaluationMatrixTableModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    QVector<QVector<QPair<QPair<int, int>, QVector<int>>>> evaluation_data;
    QMap<int, QVector<QPair<int, int>>> solutions_using_item;
    QVector<int> weights;
    QVector<int> values;
    int max_weight = 10;
public:
    EvaluationMatrixTableModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {

    }

    void LoadDataFromFile(QString file_path) {
        QFile file(file_path);
        if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) return;
        weights.clear();
        values.clear();
        while (!file.atEnd()) {
            QString line = file.readLine();
            QStringList lst = line.split(" ");
            assert(lst.size() == 2);
            int value = lst[0].toInt();
            int weight = lst[1].toInt();
            this->values.push_back(value);
            this->weights.push_back(weight);
        }
    }

    void LoadData(QVector<int>& weights, QVector<int>& values) {
        this->values = values;
        this->weights = weights;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return evaluation_data.size();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        return max_weight + 1;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole) {
            if (index.row() < evaluation_data.size() && index.column() < evaluation_data[index.row()].size()) {
                return evaluation_data[index.row()][index.column()].first.first;
            }
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role == Qt::DisplayRole) {
            if (orientation == Qt::Vertical) {
                return QString("%1").arg(section);
            }
            if (orientation == Qt::Horizontal) {
                return QString("%1").arg(section);
            }
        }
        return QVariant();
    }

    void addItem(int value, int weight) {
        weights.push_back(weight);
        values.push_back(value);
    }

    void setMaxWeight(int w) {
        this->max_weight = w;
    }

    void recalculateMatrix() {
        int nb_elements = weights.size();
        QVector<QVector<int>> best_values, best_weight;
        QVector<QVector<QVector<int>>> best_solution_items;
        best_values.push_back(QVector<int>(max_weight + 1, 0));
        best_values[0][0] = 0;
        best_weight.push_back(QVector<int>(max_weight + 1, 0));
        best_solution_items.push_back(QVector<QVector<int>>(max_weight + 1, QVector<int>()));

        for (int i = 0; i < nb_elements; ++i) {
            best_values.push_back(QVector<int>(max_weight + 1, -MAX_INT));
            best_values[i + 1][0] = 0;
            best_weight.push_back(QVector<int>(max_weight + 1, 0));
            best_solution_items.push_back(QVector<QVector<int>>(max_weight + 1, QVector<int>()));
        }
        for (int i = 0; i < nb_elements; ++i) {
            for (int w = 1; w <= max_weight; ++w) {
                if (weights[i] > w) {
                    best_values[i + 1][w] = best_values[i][w];
                    best_weight[i + 1][w] = best_weight[i][w];
                    best_solution_items[i + 1][w] = best_solution_items[i][w];
                }
                else if (best_values[i][w] < best_values[i][w-weights[i]] + values[i]) {
                    best_values[i + 1][w] = best_values[i][w-weights[i]] + values[i];
                    best_weight[i + 1][w] = best_weight[i][w-weights[i]] + weights[i];
                    best_solution_items[i + 1][w] = best_solution_items[i][w-weights[i]];
                    best_solution_items[i + 1][w].push_back(i);
                } else {
                    best_values[i + 1][w] = best_values[i][w];
                    best_weight[i + 1][w] = best_weight[i][w];
                    best_solution_items[i + 1][w] = best_solution_items[i][w];
                }
            }
        }
        solutions_using_item.clear();
        evaluation_data = QVector<QVector<QPair<QPair<int, int>, QVector<int>>>>(
                    best_values.size(),
                    QVector<QPair<QPair<int, int>, QVector<int>>>(max_weight + 1, { { -MAX_INT, -MAX_INT}, QVector<int>() })
                    );
        for (int i = 0; i < best_values.size(); ++i) {
            for (int j = 0; j < best_values[i].size(); ++j) {
                evaluation_data[i][j] = { { best_values[i][j], best_weight[i][j] }, best_solution_items[i][j] };
                for (int item : best_solution_items[i][j]) solutions_using_item[item].push_back({ i, j });
            }
        }
        emit layoutChanged();
    }

    int GetMaxWeight() {
        return this->max_weight;
    }

    int GetNbItems() {
        return evaluation_data.size() - 1;
    }

    int GetSolutionConsumedWeight(int i, int w) {
        return evaluation_data[i][w].first.second;
    }
    int GetSolutionValuesSum(int i, int w) {
        return evaluation_data[i][w].first.first;
    }
    QVector<int> GetUsedItems(int i, int w) {
        return evaluation_data[i][w].second;
    }

    QVector<QPair<int, int>> GetSolutionsThatUseItem(int item) {
        return solutions_using_item[item];
    }

    void clear() {
        weights.clear();
        values.clear();
        recalculateMatrix();
    }

    QVector<QPair<int, int>> getItems() {
        QVector<QPair<int, int>> items;
        for (int i = 0; i < values.size(); ++i) {
            items.append({values[i], weights[i]});
        }
        return items;
    }
};

class EvaluationMatrix : public QWidget
{
    Q_OBJECT
public:
    explicit EvaluationMatrix(QWidget *parent = 0);
    ~EvaluationMatrix();

    void addItem(int value, int weight) {
        table_model->addItem(value, weight);
    }

    void calculateKnapsack(int max_weight = -1) {
        if (max_weight != -1) {
            table_model->setMaxWeight(max_weight);
        }
        table_model->recalculateMatrix();
    }
    void loadFile(QString file_path) {
        table_model->LoadDataFromFile(file_path);
    }

    void clear() {
        table_model->clear();
    }

    QVector<QPair<int, int>> getItems() {
        return table_model->getItems();
    }

    void deleteItems(QSet<int> items_indexes) {
        QVector<QPair<int, int>> items = getItems();
        clear();
        for (int i = 0; i < items.size(); ++i) {
            if (!items_indexes.contains(i)) {
                addItem(items[i].first, items[i].second);
            }
        }
    }
private:
    Ui::EvaluationMatrix *ui;
    QTableView* evaluation_matrix_table = nullptr;
    EvaluationMatrixTableModel* table_model = nullptr;
    void SetSelectionModel(QItemSelectionModel* selection_model) {
        evaluation_matrix_table->setSelectionModel(selection_model);
        QObject::connect(selection_model, SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
                         this, SLOT(knapsackSolutionSelected(const QModelIndex&, const QModelIndex&)));
    }
public slots:
    void knapsackSolutionSelected(const QModelIndex & current, const QModelIndex & previous) {
        std::cout << "EvaluationMatrixTableModel::" << __func__ << std::endl;
        int consumed_weight = table_model->GetSolutionConsumedWeight(current.row(), current.column());
        int values_sum = table_model->GetSolutionValuesSum(current.row(), current.column());
        QVector<int> used_items = table_model->GetUsedItems(current.row(), current.column());
        emit knapsackSolutionSelectedSignal(QString("%1").arg(values_sum), QString("%1").arg(consumed_weight), used_items);
    }
    void ItemsSelected(QVector<int> selected_items) {
        std::cout << "EvaluationMatrixTableModel::" << __func__ << std::endl;
        QItemSelection items_selection;
        evaluation_matrix_table->selectionModel()->clearSelection();
        for (int item : selected_items) {
            for (QPair<int, int> p : table_model->GetSolutionsThatUseItem(item)) {
                int& i = p.first;
                int& w = p.second;
                QModelIndex index = table_model->index(i, w);
                evaluation_matrix_table->selectionModel()->select(index, QItemSelectionModel::Select);
                items_selection.merge(evaluation_matrix_table->selectionModel()->selection(), QItemSelectionModel::Select);
            }
        }

        QItemSelectionModel* items_selection_model = new QItemSelectionModel(this->table_model, this);
        items_selection_model->clear();
        items_selection_model->select(items_selection, QItemSelectionModel::Select);
        this->SetSelectionModel(items_selection_model);
//        evaluation_matrix_table->selectionModel()->select(items_selection, QItemSelectionModel::Select);
    }
signals:
    void knapsackSolutionSelectedSignal(QString values_sum, QString consumed_weight, QVector<int> used_items);
};

#endif // EVALUATIONMATRIX_H
