#include "evaluationmatrix.h"
#include "ui_evaluationmatrix.h"

EvaluationMatrix::EvaluationMatrix(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EvaluationMatrix)
{
    ui->setupUi(this);
    this->

    evaluation_matrix_table = ui->evaluation_matrix_table;
    table_model = new EvaluationMatrixTableModel(evaluation_matrix_table);
    evaluation_matrix_table->setModel(table_model);
    evaluation_matrix_table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    evaluation_matrix_table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

    QItemSelectionModel* selection_model = evaluation_matrix_table->selectionModel();
    QObject::connect(selection_model, SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
                     this, SLOT(knapsackSolutionSelected(const QModelIndex&, const QModelIndex&)));
}

EvaluationMatrix::~EvaluationMatrix()
{
    delete ui;
}
