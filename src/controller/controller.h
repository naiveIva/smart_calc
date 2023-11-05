#ifndef SMARTCALC_CONTROLLER_H_
#define SMARTCALC_CONTROLLER_H_

#include "../model/model.h"

namespace s21 {
/**
 * @brief Класс контроллера.
 */
class Controller {
 public:
  Controller() = default;

  ~Controller() = default;

  double CalculateValue(std::string &expression, std::string &x);

  void GetDataForGraph(std::string &expression, double x_min, double x_max,
                       std::vector<double> &x_data,
                       std::vector<double> &y_data);

 private:
  PolishNotation model_;
};
}  // namespace s21

#endif  // SMARTCALC_CONTROLLER_H_