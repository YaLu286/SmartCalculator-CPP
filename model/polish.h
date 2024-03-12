#ifndef CPP3_SMARTCALC_V2_MODEL_POLISH_H
#define CPP3_SMARTCALC_V2_MODEL_POLISH_H

#include <cmath>
#include <iostream>
#include <set>
#include <stack>
#include <vector>

namespace s21 {
/// @brief класс, конвертирующий строковые математические выражения в инфиксной
/// нотации в стек в формате польской записи для дальнейших расчетов
class PolishNotation {
 public:
  /// Перечисление типов элементов выражения
  enum Priority {
    OpeningBracket = -1,  ///< открывающая скобка
    ClosingBracket = 0,   ///< закрывающая скобка
    Number = 1,           ///< число
    BinaryLow = 2,  ///< бинарный оператор меньшей приоритетности
    BinaryHigh = 3,  ///< бинарный оператор большей приоритетности
    Unary = 4        ///< унарный оператор
  };
  /// Вложенный класс, хранящий информацию о эелементе выражения
  class Item;
  // Базовый конструктор польской записи
  PolishNotation();
  // Конструктор копирования
  PolishNotation(const PolishNotation &other);
  // Перегрузка оператора присваивания(корирование)
  PolishNotation operator=(const PolishNotation &other);
  // Деструктор
  ~PolishNotation();
  /// Принимает выражение в инфиксной записи в формате std::string,
  /// вызывает приватные методы для конвертации его в стэк в формате польской
  /// записи
  void LoadExpression(const std::string &expression);
  /// Вычисляет значение выражения
  double CalculateVal();
  /// Вычисляет значение уравнения с заданной переменной х
  double CalculateX(double x);
  /// Вычисляет вектор значений для заданного вектора переменных
  std::vector<double> CalculateVectorX(const std::vector<double> &vector_x);

 private:
  /// Разделяет лексемы в строке символом '!'
  std::string SeparateExpression(const std::string &expression);
  /// Собирает выражение в формате польской записи
  void ConvertExpressionToPolishNotation(const std::string &str);
  ///  Разделяет строку на вектор лексем
  std::vector<std::string> SplitStringToVector(std::string delim,
                                               const std::string &src);
  /// бинарная операция
  double BinaryOperation(double arg1, double arg2, const std::string &op);
  /// унарная операция
  double UnaryOperation(double arg, const std::string &op);
  /// указатель на стек элементов
  std::stack<Item> *s_;
};

class PolishNotation::Item {
 public:
  /// @brief Стандартный конструктор элемента
  Item(int type, double val, const std::string &tok);
  /// @brief Конструктор элемента из лексемы
  /// @param token лексема выражения
  explicit Item(const std::string &token);
  int get_type();
  double get_value();
  void set_value(double x);
  std::string get_tok();
  bool IsOperation();
  bool IsNumber();
  bool IsNotNumber();

 private:
  int type_;  ///< тип элемента выражения (его приоритетность)
  double value_;     ///< значение аргумента выражения
  std::string tok_;  ///< строчное представление элемента выражения
};
}  // namespace s21
#endif