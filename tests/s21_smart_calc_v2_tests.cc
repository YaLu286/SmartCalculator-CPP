#include <gtest/gtest.h>

#include "../model/polish.h"

TEST(S21SmartCalcV2, test1) {
  std::string expression = "(2.5+2.5-1.0)*2+3/3";
  double res = 0.0f;
  s21::PolishNotation pn;
  pn.LoadExpression(expression);
  res = pn.CalculateVal();
  double exp_res = 9.0f;
  ASSERT_NEAR(exp_res, res, 10e-7);
}

TEST(S21SmartCalcV2, test2) {
  std::string expression = "(sin(2.5)*cos(2.5)-ln(1.0))*0.2+((3/3)*(7^2))";
  double res = 0.0f;
  s21::PolishNotation pn;
  pn.LoadExpression(expression);
  res = pn.CalculateVal();
  double exp_res = 48.904107572;
  ASSERT_NEAR(exp_res, res, 10e-7);
}

TEST(S21SmartCalcV2, test3) {
  std::string expression = "asin(1)+atan(9)*acos(0.5)+x";
  double res = 0.0f;
  s21::PolishNotation pn;
  pn.LoadExpression(expression);
  res = pn.CalculateX(0);
  double exp_res = 3.09985042f;
  ASSERT_NEAR(exp_res, res, 10e-7);
}

TEST(S21SmartCalcV2, test4) {
  std::string expression = "log(-4)";
  double res = 0.0f;
  s21::PolishNotation pn;
  pn.LoadExpression(expression);
  res = pn.CalculateVal();
  ASSERT_TRUE(res != res);
}

TEST(S21SmartCalcV2, test5) {
  std::string expression = "((-3)%2)+sqrt(+4)";
  double res = 0.0f;
  s21::PolishNotation pn;
  pn.LoadExpression(expression);
  res = pn.CalculateVal();
  double exp_res = 1.0f;
  ASSERT_NEAR(exp_res, res, 10e-7);
}

TEST(S21SmartCalcV2, test6) {
  std::string expression = "tan(4*-5)+0.02";
  double res = 0.0f;
  s21::PolishNotation pn;
  pn.LoadExpression(expression);
  res = pn.CalculateVal();
  double exp_res = -2.2171609f;
  ASSERT_NEAR(exp_res, res, 10e-7);
}

TEST(S21SmartCalcV2, exeption_test1) {
  std::string expression = "tan(4***-5)+0.02-+*";
  s21::PolishNotation pn;
  EXPECT_ANY_THROW(pn.LoadExpression(expression); pn.CalculateVal(););
}

TEST(S21SmartCalcV2, exeption_test2) {
  std::string expression = "sin(error)-5+0.02";
  s21::PolishNotation pn;
  EXPECT_ANY_THROW(pn.LoadExpression(expression); pn.CalculateVal(););
}

TEST(S21SmartCalcV2, exeption_test3) {
  std::string expression = "sin(x^2)))";
  s21::PolishNotation pn;
  EXPECT_ANY_THROW(pn.LoadExpression(expression); pn.CalculateVal(););
}

TEST(S21SmartCalcV2, exeption_test4) {
  std::string expression = "sin(+)";
  s21::PolishNotation pn;
  EXPECT_ANY_THROW(pn.LoadExpression(expression); pn.CalculateVal(););
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
