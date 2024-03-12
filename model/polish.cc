#include "polish.h"

namespace s21 {

PolishNotation::PolishNotation() { s_ = new std::stack<Item>; }

PolishNotation::PolishNotation(const PolishNotation &other) { *this = other; }

PolishNotation PolishNotation::operator=(const PolishNotation &other) {
  s_ = new std::stack<Item>;
  *s_ = *other.s_;
  return *this;
}

PolishNotation::~PolishNotation() { delete s_; }

void PolishNotation::LoadExpression(const std::string &expression) {
  ConvertExpressionToPolishNotation(SeparateExpression(expression));
}

std::vector<double> PolishNotation::CalculateVectorX(
    const std::vector<double> &vector_x) {
  std::vector<double> res_vector(vector_x.size());
  auto it_x = vector_x.begin();
  auto res_it = res_vector.begin();
  for (; it_x != vector_x.end(); it_x++, res_it++) {
    *res_it = CalculateX(*it_x);
  }
  return res_vector;
}

double PolishNotation::CalculateVal() { return CalculateX(1.0f); }

std::string PolishNotation::SeparateExpression(const std::string &expr) {
  std::string res;
  for (size_t i = 0; expr[i]; i++) {
    if (expr[i] == '+' && !isdigit(expr[i - 1]) && expr[i - 1] != 'x' &&
        expr[i - 1] != ')') {
    } else if (expr[i] == '-' && !isdigit(expr[i - 1]) && expr[i - 1] != 'x' &&
               expr[i - 1] != ')') {
      res.resize(res.length() + 1, '~');
      res.resize(res.length() + 1, '!');
    } else if ((expr[i] == 'n' || expr[i] == 's' || expr[i] == 'g' ||
                (expr[i] == 't' && (expr[i + 1] != 'a')) || expr[i] == '+' ||
                expr[i] == '-' || expr[i] == '%' || expr[i] == '*' ||
                expr[i] == '/' || expr[i] == '^' || expr[i] == '(' ||
                expr[i] == ')') &&
               (expr[i + 1] != 'i') && (expr[i + 1] != 'q')) {
      res.resize(res.length() + 1, expr[i]);
      res.resize(res.length() + 1, '!');
    } else if (((expr[i] >= '0' && expr[i] <= '9') || expr[i] == 'x') &&
               !(expr[i + 1] >= '0' && expr[i + 1] <= '9') &&
               expr[i + 1] != '.') {
      res.resize(res.length() + 1, expr[i]);
      res.resize(res.length() + 1, '!');
    } else {
      res.resize(res.length() + 1, expr[i]);
    }
  }
  return res;
}

std::vector<std::string> PolishNotation::SplitStringToVector(
    std::string delim, const std::string &src) {
  std::string::size_type p, start = 0, len = src.length();
  std::vector<std::string> v;
  start = src.find_first_not_of(delim);
  p = src.find_first_of(delim, start);
  while (p != std::string::npos) {
    v.push_back(src.substr(start, p - start));
    start = src.find_first_not_of(delim, p);
    p = src.find_first_of(delim, start);
  }
  if (len > start) v.push_back(src.substr(start, len - start));
  return v;
}

void PolishNotation::ConvertExpressionToPolishNotation(const std::string &str) {
  std::stack<Item> stack1;
  std::stack<Item> stack2;
  std::vector<std::string> tokens = SplitStringToVector("!", str);
  for (auto it = tokens.begin(); it != tokens.end(); it++) {
    Item elem(*it);
    if (elem.IsNumber()) {
      stack1.push(elem);
    } else if (elem.IsOperation()) {
      while (!stack2.empty() && stack2.top().get_type() > elem.get_type()) {
        Item tmp = stack2.top();
        stack2.pop();
        stack1.push(tmp);
      }
      stack2.push(elem);
    } else if (elem.get_type() == OpeningBracket) {
      stack2.push(elem);
    } else if (elem.get_type() == ClosingBracket) {
      while (!stack2.empty() && stack2.top().get_type() != OpeningBracket) {
        Item tmp = stack2.top();
        stack2.pop();
        stack1.push(tmp);
      }
      if (!stack2.empty()) {
        stack2.pop();
      } else {
        throw std::invalid_argument("Bad expression!");
      }
    }
  }
  while (!stack2.empty()) {
    Item tmp = stack2.top();
    stack2.pop();
    stack1.push(tmp);
  }
  *s_ = stack1;
}

double PolishNotation::CalculateX(double x) {
  double result = 0;
  std::stack<Item> stack1(*s_);
  std::stack<Item> stack2;
  while (!stack1.empty()) {
    Item tmp = stack1.top();
    if (tmp.get_tok() == "x") {
      tmp.set_value(x);
    }
    stack1.pop();
    stack2.push(tmp);
  }
  while (stack2.size() > 1) {
    while (!stack2.empty() && stack2.top().get_type() < 2) {
      Item tmp = stack2.top();
      stack2.pop();
      stack1.push(tmp);
    }
    if (!stack2.empty() && stack2.top().get_type() < 4) {
      Item op = stack2.top();
      stack2.pop();
      Item arg2{0, 0, ""};
      if (!stack1.empty()) {
        arg2 = stack1.top();
        stack1.pop();
      }
      Item arg1{0, 0, ""};
      if (!stack1.empty()) {
        arg1 = stack1.top();
        stack1.pop();
      }
      if (arg2.IsNotNumber() || arg1.IsNotNumber()) {
        throw std::invalid_argument("Bad expression!");
      } else {
        Item res{
            Number,
            BinaryOperation(arg1.get_value(), arg2.get_value(), op.get_tok()),
            " "};
        stack2.push(res);
      }
    } else if (!stack2.empty() && stack2.top().get_type() == Unary) {
      Item op = stack2.top();
      stack2.pop();
      Item arg{0, 0, ""};
      if (!stack1.empty()) {
        arg = stack1.top();
        stack1.pop();
      }
      if (arg.IsNotNumber()) {
        throw std::invalid_argument("Bad expression!");
      } else {
        Item res(Number, UnaryOperation(arg.get_value(), op.get_tok()),
                 std::string(" "));
        stack2.push(res);
      }
    } else {
      throw std::invalid_argument("Bad expression!");
    }
    while (!stack1.empty()) {
      Item tmp = stack1.top();
      stack1.pop();
      stack2.push(tmp);
    }
  }
  if (stack2.top().IsNumber()) result = stack2.top().get_value();
  return result;
}

double PolishNotation::BinaryOperation(double arg1, double arg2,
                                       const std::string &op) {
  double res = 0;
  if (op == "+") {
    res = arg1 + arg2;
  } else if (op == "-") {
    res = arg1 - arg2;
  } else if (op == "*") {
    res = arg1 * arg2;
  } else if (op == "/") {
    res = arg1 / arg2;
  } else if (op == "^") {
    res = pow(arg1, arg2);
  } else if (op == "%") {
    res = fmod(arg1, arg2);
  }
  return res;
}

double PolishNotation::UnaryOperation(double arg, const std::string &op) {
  double res = 0;
  if (op == "~") {
    res = arg * -1;
  } else if (op == "sin") {
    res = sin(arg);
  } else if (op == "cos") {
    res = cos(arg);
  } else if (op == "tan") {
    res = tan(arg);
  } else if (op == "asin") {
    res = asin(arg);
  } else if (op == "acos") {
    res = acos(arg);
  } else if (op == "atan") {
    res = atan(arg);
  } else if (op == "ln") {
    res = log(arg);
  } else if (op == "log") {
    res = log10(arg);
  } else if (op == "sqrt") {
    res = sqrt(arg);
  }
  return res;
}

PolishNotation::Item::Item(int type, double val, const std::string &tok)
    : type_(type), value_(val), tok_(tok) {}

PolishNotation::Item::Item(const std::string &token) {
  std::set<std::string> unary(
      {"~", "sin", "cos", "tan", "asin", "acos", "atan", "sqrt", "ln", "log"});
  std::set<std::string> binary_high_priority({"*", "/", "^", "%"});
  std::set<std::string> binary_low_priority({"+", "-"});
  char *ptr_end;
  value_ = strtod(token.c_str(), &ptr_end);
  if (*ptr_end == '\0') {
    type_ = Number;
  } else if (token == "x") {
    type_ = Number;
  } else if (unary.find(token) != unary.end()) {
    type_ = Unary;
  } else if (binary_high_priority.find(token) != binary_high_priority.end()) {
    type_ = BinaryHigh;
  } else if (binary_low_priority.find(token) != binary_low_priority.end()) {
    type_ = BinaryLow;
  } else if (token == "(") {
    type_ = OpeningBracket;
  } else if (token == ")") {
    type_ = ClosingBracket;
  } else {
    throw std::invalid_argument("Bad expression!");
  }
  tok_ = token;
}

bool PolishNotation::Item::IsOperation() {
  return (type_ >= BinaryLow && type_ <= Unary);
}

bool PolishNotation::Item::IsNumber() { return (type_ == Number); }

bool PolishNotation::Item::IsNotNumber() { return !(type_ == Number); }

int PolishNotation::Item::get_type() { return type_; }

double PolishNotation::Item::get_value() { return value_; }

void PolishNotation::Item::set_value(double x) { value_ = x; }

std::string PolishNotation::Item::get_tok() { return tok_; }

}  // namespace s21