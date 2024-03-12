#ifndef CPP3_SMARTCALC_V2_CONTROLLER_CALC_CONTROLLER_H
#define CPP3_SMARTCALC_V2_CONTROLLER_CALC_CONTROLLER_H

#include <QMainWindow>

#include "../model/polish.h"

namespace s21 {
/// @brief класс контроллера модели PolisNotation
class CalcController {
 public:
  explicit CalcController(PolishNotation *pn) : pn_model_(pn){};
  ~CalcController(){};
  /// @brief вычисляет значения выражения
  double CalculateResultOfExpression(const std::string &expression);
  /// @brief вычисляет значения уравнения для заданного значения переменной х
  double CalculateResultOfEquation(const std::string &equation, double x);
  /// @brief вычисляет вектор значений уравнения для заданного интервала
  /// значений переменной х
  std::pair<QVector<double>, QVector<double>> CalculateResultQVectorOfEquation(
      const std::string &equation, int x_min, int x_max, double x_frac);

 private:
  PolishNotation *pn_model_;  ///< указатель на модель PolishNotation
};
}  // namespace s21
#endif
