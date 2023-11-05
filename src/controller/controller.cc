#include "controller.h"

namespace s21 {

/**
 * @brief Вычисление выражения и передача ответа.
 * @param exppression Строка с выражением для вычисления.
 * @param x Строка со значением икса.
 * @throw std::invalid_argument В случае некорректности строки.
 * @return Число - вычисленный ответ.
 */
double Controller::CalculateValue(std::string &expression, std::string &x) {
  model_.Calculate(expression, x);
  return model_.GetAnswer();
}

/**
 * @brief Вычисление значений для пострения графика.
 * @param input_exppression Строка с выражением для вычисления.
 * @param x_min Минимальное значение икса.
 * @param x_max Максимальное значение икса.
 * @param x_data Вектор для выисленных значений X для построения графика.
 * @param y_data Вектор для выисленных значений Y для построения графика.
 * @throw std::invalid_argument В случае некорректности строки.
 */
void Controller::GetDataForGraph(std::string &expression, double x_min,
                                 double x_max, std::vector<double> &x_data,
                                 std::vector<double> &y_data) {
  model_.GetGraph(expression, x_min, x_max, x_data, y_data);
}

}  // namespace s21