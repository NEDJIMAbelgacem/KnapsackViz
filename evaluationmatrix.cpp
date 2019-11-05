#include "evaluationmatrix.h"
#include "ui_evaluationmatrix.h"

EvaluationMatrix::EvaluationMatrix(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EvaluationMatrix)
{
    ui->setupUi(this);
}

EvaluationMatrix::~EvaluationMatrix()
{
    delete ui;
}
