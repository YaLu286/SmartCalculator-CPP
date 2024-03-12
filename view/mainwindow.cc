#include "mainwindow.h"

#include "ui_mainwindow.h"

namespace s21 {
MainWindow::MainWindow(QWidget *parent, CalcController *c)
    : QMainWindow(parent), ui_(new Ui::MainWindow), ctrl_(c) {
  ui_->setupUi(this);

  // simple way to connect all pushbutton signals to one slot (QList)
  QList<QPushButton *> buttons = findChildren<QPushButton *>();
  for (auto button : buttons) {
    connect(button, &QPushButton::released, this, &MainWindow::Pushed);
  }

  SetToDefault();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::SetToDefault() {
  ui_->textEdit->setText("0");
  ui_->XvalText->setText("-");

  ui_->plot->xAxis->setLabel("x");
  ui_->plot->yAxis->setLabel("y");
  ui_->plot->xAxis->setRange(-5, 5);
  ui_->plot->yAxis->setRange(-5, 5);

  ui_->XminText->setText("-5");
  ui_->XmaxText->setText("5");
  ui_->YminText->setText("-5");
  ui_->YmaxText->setText("5");

  // set mask for x, y (min/max) fields
  ui_->XvalText->setInputMask("########");
  ui_->XminText->setInputMask("########");
  ui_->XmaxText->setInputMask("########");
  ui_->YminText->setInputMask("########");
  ui_->YmaxText->setInputMask("########");
}

void MainWindow::Pushed() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  QString str = ui_->textEdit->toPlainText();
  std::string expression = str.toStdString();
  if (button->text() == "=") {
    if (str.size() <= 255) {
      try {
        if (str.contains('x')) {
          if (ui_->XvalText->text() != "-") {
            double x = ui_->XvalText->text().toDouble();
            double res_y = ctrl_->CalculateResultOfEquation(expression, x);
            ui_->textEdit->setText(QString::number(res_y, 'f', 6));
          }
          DrawPlot(expression);
        } else {
          double res = ctrl_->CalculateResultOfExpression(expression);
          ui_->textEdit->setText(QString::number(res, 'f', 6));
        }
      } catch (...) {
        ui_->textEdit->setText("Bad expression!");
      }
    } else {
      ui_->textEdit->setText("Expression is too large!");
    }
  } else if (button->text() == "AC") {
    ui_->textEdit->setText("0");
  } else {
    if (str == "0") {
      str.clear();
    }
    str += button->text();
    ui_->textEdit->setPlainText(str);
  }
}

void MainWindow::DrawPlot(std::string equation) {
  int x_min, x_max, y_min, y_max;
  ValidateIntervals(x_min, x_max, y_min, y_max);
  std::pair<QVector<double>, QVector<double>> graph_data =
      ctrl_->CalculateResultQVectorOfEquation(equation, x_min, x_max,
                                              X_FRACTION);
  ui_->plot->addGraph();
  ui_->plot->graph(0)->setData(graph_data.first, graph_data.second);
  ui_->plot->graph(0)->setLineStyle(QCPGraph::lsNone);
  ui_->plot->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssDisc, Qt::blue, 1));
  ui_->plot->xAxis->setLabel("x");
  ui_->plot->yAxis->setLabel("y");
  ui_->plot->xAxis->setRange(x_min, x_max);
  ui_->plot->yAxis->setRange(y_min, y_max);
  ui_->plot->replot();
}

void MainWindow::ValidateIntervals(int &x_min, int &x_max, int &y_min,
                                   int &y_max) {
  x_min = ui_->XminText->text().toInt();
  x_max = ui_->XmaxText->text().toInt();
  y_min = ui_->YminText->text().toInt();
  y_max = ui_->YmaxText->text().toInt();

  if (x_min < -1000000) x_min = -1000000;

  if (x_max > 1000000) x_max = 1000000;

  if (y_min < -1000000) y_min = -1000000;

  if (y_max > 1000000) y_max = 1000000;

  if (x_min > x_max) std::swap(x_min, x_max);

  if (y_min > y_max) std::swap(y_min, y_max);

  ui_->XminText->setText(QString::number(x_min));
  ui_->XmaxText->setText(QString::number(x_max));
  ui_->YminText->setText(QString::number(y_min));
  ui_->YmaxText->setText(QString::number(y_max));
}
}  // namespace s21
