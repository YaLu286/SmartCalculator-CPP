#include "calc_controller.h"

namespace s21 {

double CalcController::CalculateResultOfExpression(
    const std::string &expression) {
  pn_model_->LoadExpression(expression);
  return pn_model_->CalculateVal();
}

double CalcController::CalculateResultOfEquation(const std::string &equation,
                                                 double x) {
  pn_model_->LoadExpression(equation);
  return pn_model_->CalculateX(x);
}

std::pair<QVector<double>, QVector<double>>
CalcController::CalculateResultQVectorOfEquation(const std::string &equation,
                                                 int x_min, int x_max,
                                                 double x_frac) {
  int vector_size = (x_max - x_min) / x_frac;

  pn_model_->LoadExpression(equation);

  QVector<double> Q_x(vector_size);

  Q_x[0] = x_min;
  for (int i = 1; i < vector_size; ++i) {
    Q_x[i] = Q_x[i - 1] + x_frac;
  }

  std::vector<double> std_x = std::vector<double>(Q_x.begin(), Q_x.end());

  std::vector<double> std_y = pn_model_->CalculateVectorX(std_x);

  QVector<double> Q_y = QVector<double>(std_y.begin(), std_y.end());

  return std::pair(Q_x, Q_y);
}

}  // namespace s21
