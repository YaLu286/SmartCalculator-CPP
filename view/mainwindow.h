#ifndef CPP3_SMARTCALC_V2_VIEWS_MAINWINDOW_H
#define CPP3_SMARTCALC_V2_VIEWS_MAINWINDOW_H

#define X_FRACTION 0.01

#include <QMainWindow>
#include <string>

#include "../controller/calc_controller.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
namespace s21 {
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent, CalcController *c);
  ~MainWindow();
  void SetToDefault();
  void ValidateIntervals(int &x_min, int &x_max, int &y_min, int &y_max);
  void Pushed();
  void DrawPlot(std::string expression);

 private:
  Ui::MainWindow *ui_;
  CalcController *ctrl_;
};
}  // namespace s21
#endif  // MAINWINDOW_H
