#ifndef SMARTCALC_MODEL_H_
#define SMARTCALC_MODEL_H_

#include <cctype>
#include <cmath>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <stack>
#include <utility>
#include <variant>
#include <vector>

namespace s21 {

/**
 * @brief Класс для обработки и вычисления выражения через польскую нотацию.
 */
class PolishNotation {
  using unary_function = std::function<double(double)>;
  using binary_function = std::function<double(double, double)>;
  using function_variant =
      std::variant<double, unary_function, binary_function, nullptr_t>;

 public:
  PolishNotation() = default;

  ~PolishNotation();

  void Calculate(std::string &input_expression, std::string &x_value);

  void GetGraph(std::string &input_expression, double x_min, double x_max,
                std::vector<double> &x_data, std::vector<double> &y_data);

  double GetAnswer() const;

 private:
  /**
   * @brief Перечисление групп лексем.
   */
  enum Group {
    g_number,      ///< Число
    g_unary_op,    ///< Унарный оператор
    g_binary_op,   ///< Бинарный оператор
    g_function,    ///< Функиця
    g_opening_br,  ///< Открывающая скобка
    g_closing_br,  ///< Закрывающая скобка
    g_none         ///< Не определена
  };

  /**
   * @brief Перечисление типов лексем.
   */
  enum Type {
    t_cos,
    t_sin,
    t_tan,
    t_acos,
    t_asin,
    t_atan,
    t_ln,
    t_log,
    t_sqrt,
    t_pow,
    t_mult,
    t_div,
    t_mod,
    t_plus,
    t_minus,
    t_unary_plus,
    t_unary_minus,
    t_opening_br,
    t_closing_br,
    t_number,
    t_x,
    t_none
  };

  /**
   * @brief Подкласс для отдельной лексемы.
   */
  class Lexema {
   public:
    Lexema(const std::string &name, Type type, Group group, int prior,
           function_variant function);

    ~Lexema() = default;

    Type GetType() const;

    Group GetGroup() const;

    int GetPrioroty() const;

    auto GetFunction() const;

    std::string GetName() const;

   private:
    std::string name_;  ///< Название лексемы.

    Type type_;  ///< Тип лексемы.

    Group group_;  ///< Группа лексемы.

    int priority_;  ///< Приориет лексемы.

    function_variant function_;  ///< Функция или число лексемы.
  };

  double x;  ///< Значение икса для подстановки в выражение.

  bool x_is_found = false;  ///< Флаг, обозначающий, что икс присутствует в
                            ///< строке с выражением.

  double final_answer;  ///< Вычисленный ответ.

  std::deque<Lexema> parsed_lexemas_;  ///< Очередь с прочитанными лексемами.

  std::stack<Lexema> stack_of_numbers_;  ///< Стэк с числами из выражения.

  std::stack<Lexema> stack_of_operators_;  ///< Стэк с нечислами из выражения
                                           ///< (операторы, функции, скобки).

  void ToLowerCase(std::string &input_expression);

  void ParseExpression(std::string &input_expression);

  double ParseNumber(std::string::iterator &iter);

  void ParseLetter(std::string::iterator &iter);

  void ParseSymbol(std::string::iterator &iter, int &brackets_count);

  double ParseX(std::string &x_value);

  bool CompareIterToStr(std::string::iterator &iter, const std::string &str);

  bool IsUnary();

  Lexema FindLexemaInMap(Type type);

  void PushOperatorToDeque(Type type);

  void PushNumberToDeque(double number);

  void PushXToDeque();

  void ValidateFirstLexema();

  void ValidateLastLexema();

  void ValidateParsedLexemas(int brackets_count);

  void LexemasProcessing();

  double GetNumberFromStack();

  Lexema GetOperationFromStack();

  void MakeOperation(Lexema &lex);

  void ClosingBracketProcessing();

  void FinalCalculations();

  void ClearAll();

  /**
  Словарь для нахождения лексемы по типу.
  Ключ - тип лексемы, значение - готовая лексема.
  */
  std::map<Type, Lexema> lexema_map_ = {
      {t_cos,
       Lexema("cos", t_cos, g_function, 4, [](double n) { return cos(n); })},
      {t_sin,
       Lexema("sin", t_sin, g_function, 4, [](double n) { return sin(n); })},
      {t_tan,
       Lexema("tan", t_tan, g_function, 4, [](double n) { return tan(n); })},
      {t_acos,
       Lexema("acos", t_acos, g_function, 4, [](double n) { return acos(n); })},
      {t_asin,
       Lexema("asin", t_asin, g_function, 4, [](double n) { return asin(n); })},
      {t_atan,
       Lexema("atan", t_atan, g_function, 4, [](double n) { return atan(n); })},
      {t_ln,
       Lexema("ln", t_ln, g_function, 4, [](double n) { return log(n); })},
      {t_log,
       Lexema("log", t_log, g_function, 4, [](double n) { return log10(n); })},
      {t_pow, Lexema("^", t_pow, g_binary_op, 3,
                     [](double n, double m) { return pow(n, m); })},
      {t_sqrt,
       Lexema("sqrt", t_sqrt, g_function, 3, [](double n) { return sqrt(n); })},
      {t_mod, Lexema("mod", t_mod, g_binary_op, 2,
                     [](double n, double m) { return fmod(n, m); })},

      {t_mult, Lexema("*", t_mult, g_binary_op, 2, std::multiplies<double>())},
      {t_div, Lexema("/", t_div, g_binary_op, 2, std::divides<double>())},
      {t_plus, Lexema("+", t_plus, g_binary_op, 1, std::plus<double>())},
      {t_minus, Lexema("-", t_minus, g_binary_op, 1, std::minus<double>())},

      {t_opening_br, Lexema("(", t_opening_br, g_opening_br, 0, nullptr)},
      {t_closing_br, Lexema(")", t_closing_br, g_closing_br, 0, nullptr)}};

  /**
  Матрица смежности для лексем. Определяет, могут ли две лексемы идти подряд в
  корректном математичсеком выражении.
  */
  const bool ValidityMatrix[6][6] = {
      {0, 0, 1, 0, 0, 1},  // number
      {1, 0, 0, 1, 1, 0},  // unary
      {1, 0, 0, 1, 1, 0},  // binary
      {0, 0, 0, 0, 1, 0},  // funcs
      {1, 1, 0, 1, 1, 0},  // (
      {0, 0, 1, 0, 0, 1}   // )
  };
};

}  // namespace s21

#endif  // SMARTCALC_MODEL_H_
