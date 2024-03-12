#include <locale.h>

#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  setlocale(LC_ALL, "en_US.utf8");
  s21::PolishNotation polish_notation_model;
  s21::CalcController calc_controller(&polish_notation_model);
  s21::MainWindow w(nullptr, &calc_controller);
  w.show();
  return a.exec();
}
