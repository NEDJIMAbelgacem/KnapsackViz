#ifndef EVALUATIONMATRIX_H
#define EVALUATIONMATRIX_H

#include <QWidget>

namespace Ui {
class EvaluationMatrix;
}

class EvaluationMatrix : public QWidget
{
    Q_OBJECT

public:
    explicit EvaluationMatrix(QWidget *parent = 0);
    ~EvaluationMatrix();

private:
    Ui::EvaluationMatrix *ui;
};

#endif // EVALUATIONMATRIX_H
