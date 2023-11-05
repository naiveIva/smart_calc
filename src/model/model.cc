#include "model.h"

namespace s21 {

PolishNotation::~PolishNotation() { ClearAll(); }

/**
 * @brief Основная функция для вычисления.
 * @param input_expression Строка с выражением.
 * @param x_value Строка, содержащая значение икса.
 * @throw std::invalid_argument В случае некорректности строки.
 * @details Подготовка, парсинг, обработка и вычисление выражения.
 */
void PolishNotation::Calculate(std::string &input_expression,
                               std::string &x_value) {
  try {
    ToLowerCase(input_expression);
    ParseExpression(input_expression);
    if (x_is_found) {
      x = ParseX(x_value);
    }
    LexemasProcessing();
    FinalCalculations();
  } catch (const std::exception &ex) {
    ClearAll();
    throw;
  }
}

/**
 * @brief Функция-геттер, возвращающая результат вычисления.
 * @return Число - результат вычисления.
 */
double PolishNotation::GetAnswer() const { return final_answer; }

/**
 * @brief Конструктор.
 * @param name Строка, содержащая данную лексему.
 * @param type Тип лексемы.
 * @param group Группа лексемы.
 * @param prior Приоритет лексемы.
 * @param functiion Функция или число, соответствующая лексеме.
 */
PolishNotation::Lexema::Lexema(const std::string &name, Type type, Group group,
                               int prior, function_variant function)
    : name_(name),
      type_(type),
      group_(group),
      priority_(prior),
      function_(function) {}

/**
 * @brief Функиця-геттер, возвращающая тип лексемы.
 */
PolishNotation::Type PolishNotation::Lexema::GetType() const { return type_; }

/**
 * @brief Функиця-геттер, возвращающая группу лексемы.
 */
PolishNotation::Group PolishNotation::Lexema::GetGroup() const {
  return group_;
}

/**
 * @brief Функиця-геттер, возвращающая приоритет лексемы.
 * @return Тип лексемы (число от 1 до 4).
 */
int PolishNotation::Lexema::GetPrioroty() const { return priority_; }

/**
 * @brief Функиця-геттер, возвращающая соответствующую лексеме функцию.
 * @return Ссылка на функцию либо лямбда-выражение.
 */
auto PolishNotation::Lexema::GetFunction() const { return function_; }

/**
 * @brief Функция-геттер, возвращающая строку с лексемой.
 */
std::string PolishNotation::Lexema::GetName() const { return name_; }

/**
 * @brief Перевод строки в нижний регистр.
 */
void PolishNotation::ToLowerCase(std::string &input_expression) {
  std::transform(input_expression.begin(), input_expression.end(),
                 input_expression.begin(), ::tolower);
}

/**
 * @brief Функция для разбивки выражения на лексемы и их валидации.
 * @param input_expression Строка с выражением.
 */
void PolishNotation::ParseExpression(std::string &input_expression) {
  std::string::iterator iter_expression = input_expression.begin();
  int brackets_count = 0;

  if (input_expression.empty()) {
    throw std::invalid_argument("Empty input");
  }

  while (iter_expression < input_expression.end()) {
    if (*iter_expression == ' ') {
      ++iter_expression;
    } else if (isdigit(*iter_expression)) {
      double num = ParseNumber(iter_expression);
      PushNumberToDeque(num);
    } else if (*iter_expression == 'x') {
      PushXToDeque();
      x_is_found = true;
      ++iter_expression;
    } else if (isalpha(*iter_expression)) {
      ParseLetter(iter_expression);
    } else {
      ParseSymbol(iter_expression, brackets_count);
    }
  }
  ValidateParsedLexemas(brackets_count);
}

/**
 * @brief Лексема (число) кладется в очередь с распаршенными лексемами.
 * @param number Число.
 */
void PolishNotation::PushNumberToDeque(double number) {
  parsed_lexemas_.push_back(
      Lexema(std::to_string(number), t_number, g_number, 0, number));
}

/**
 * @brief Икс кладется в очередь с распаршенными лексемами.
 */
void PolishNotation::PushXToDeque() {
  parsed_lexemas_.push_back(Lexema("x", t_x, g_number, 0, nullptr));
}

/**
 * @brief Парсинг числа: число вычленяется из строки с выражением и кладется в
 * очередь с лексемами.
 * @param iter Итератор в строке с выражением, указывающий на начало числа.
 * @return Значение найденного числа.
 */
double PolishNotation::ParseNumber(std::string::iterator &iter) {
  std::string number_str;
  if (*iter == '-' || *iter == '+') {
    number_str += *iter;
    iter++;
  }
  int dot_check = 0;
  int e_check = 0;
  int sign_check = 0;
  for (; isdigit(*iter) || *iter == '.' || *iter == 'e' || *iter == '-' ||
         *iter == '+';
       ++iter) {
    if (isdigit(*iter)) {
      number_str += *iter;
    } else if (*iter == '.' && !dot_check && *(iter - 1) != 'e') {
      number_str += *iter;
      dot_check = 1;
    } else if (*iter == 'e' && !e_check) {
      number_str += *iter;
      e_check = 1;
    } else if ((*iter == '+' || *iter == '-') && *(iter - 1) == 'e' &&
               !sign_check) {
      number_str += *iter;
      sign_check = 1;
    } else if (*iter == '+' || *iter == '-') {
      break;
    } else {
      throw std::invalid_argument("Incorrect number");
    }
  }
  if (*(iter - 1) == 'e' || *(iter - 1) == '-' || *(iter - 1) == '+' ||
      *(iter - 1) == '.') {
    throw std::invalid_argument("Incorrect number");
  }
  return std::stod(number_str);
}

/**
 * @brief Кастомное сравнение двух строк для проверки, содержится ли функция в
 * строке-выражении по указанному итератором адресу.
 * @param iter Итератор в строке с выражением.
 * @param str Строка для сравнения (название искомой функции).
 * @return Результат сравнения: true или false
 */
bool PolishNotation::CompareIterToStr(std::string::iterator &iter,
                                      const std::string &str) {
  std::string::iterator iter_copy = iter;
  bool result = true;
  for (size_t i = 0; i < str.size(); ++i, ++iter) {
    if (*iter != str[i]) {
      result = false;
      iter = iter_copy;
      break;
    }
  }
  return result;
}

/**
 * @brief Поиск лексемы (функции или опертора) по типу в словаре lexema_map_
 * @param type Тип искомой лексемы.
 * @return Лексема.
 */
PolishNotation::Lexema PolishNotation::FindLexemaInMap(Type type) {
  auto lexema_iter = lexema_map_.find(type);
  if (lexema_iter == lexema_map_.end()) {
    throw std::invalid_argument("Operator is not maintained");
  }
  return lexema_iter->second;
}

/**
 * @brief Лексема (функция или оператор) кладется в очередь с распаршенными
 * лексемами.
 * @param type Тип лексемы.
 */
void PolishNotation::PushOperatorToDeque(Type type) {
  parsed_lexemas_.push_back(FindLexemaInMap(type));
}

/**
 * @brief Парсинг функции: функция вычленяется из строки с выражением и
 * кладется в очередь с лексемами.
 * @param iter Итератор в строке с выражением, указывающий на начало функции.
 */
void PolishNotation::ParseLetter(std::string::iterator &iter) {
  Type result = t_none;
  switch (*iter) {
    case 'c': {
      if (CompareIterToStr(iter, "cos")) {
        result = t_cos;
      }
      break;
    }

    case 's': {
      if (CompareIterToStr(iter, "sin")) {
        result = t_sin;
      } else if (CompareIterToStr(iter, "sqrt")) {
        result = t_sqrt;
      }
      break;
    }

    case 't': {
      if (CompareIterToStr(iter, "tan")) {
        result = t_tan;
      }
      break;
    }

    case 'a': {
      if (CompareIterToStr(iter, "acos")) {
        result = t_acos;
      } else if (CompareIterToStr(iter, "asin")) {
        result = t_asin;
      } else if (CompareIterToStr(iter, "atan")) {
        result = t_atan;
      }
      break;
    }

    case 'l': {
      if (CompareIterToStr(iter, "ln")) {
        result = t_ln;
      } else if (CompareIterToStr(iter, "log")) {
        result = t_log;
      }
      break;
    }

    case 'm': {
      if (CompareIterToStr(iter, "mod")) {
        result = t_mod;
      }
      break;
    }

    default: {
      throw std::invalid_argument("Incorrect input");
    }
  }
  if (result == t_none) {
    throw std::invalid_argument("Incorrect input");
  }
  PushOperatorToDeque(result);
}

/**
 * @brief Определение, является ли оператор унарным.
 * @return Результат определения: true - если оператор унарный, false - если
 * не унарный.
 */
bool PolishNotation::IsUnary() {
  if (parsed_lexemas_.empty() ||
      parsed_lexemas_.back().GetType() == t_opening_br) {
    return true;
  }
  return false;
}

/**
 * @brief Парсинг оператора: оператор вычленяется из строки с выражением и
 * кладется в очередь с лексемами.
 * @param iter Итератор в строке с выражением, указывающий на символ
 * оператора.
 */
void PolishNotation::ParseSymbol(std::string::iterator &iter,
                                 int &brackets_count) {
  Type result = t_none;
  switch (*iter) {
    case '+': {
      if (IsUnary()) {
        PushNumberToDeque(0);
      }
      result = t_plus;
      break;
    }
    case '-': {
      if (IsUnary()) {
        PushNumberToDeque(0);
      }
      result = t_minus;
      break;
    }
    case '*': {
      result = t_mult;
      break;
    }
    case '/': {
      result = t_div;
      break;
    }
    case '^': {
      result = t_pow;
      break;
    }
    case '(': {
      result = t_opening_br;
      ++brackets_count;
      break;
    }
    case ')': {
      result = t_closing_br;
      --brackets_count;
      break;
    }
    default: {
      throw std::invalid_argument("Incorrect symbol in input expression");
    }
  }
  if (result == t_none) {
    throw std::invalid_argument("Incorrect symbol in input expression");
  }
  iter++;
  PushOperatorToDeque(result);
}

/**
 * @brief Парсинг и валидация значения икса.
 * @param iter Строка со значением икса.
 * @return Значение икс.
 */
double PolishNotation::ParseX(std::string &x_value) {
  if (x_value.empty()) {
    throw std::invalid_argument("X value is empty");
  }
  ToLowerCase(x_value);
  std::string::iterator x_iter = x_value.begin();
  double result;
  try {
    result = ParseNumber(x_iter);
  } catch (const std::invalid_argument &ex) {
    throw std::invalid_argument("Invalid X value");
  }
  if (x_iter != x_value.end()) {
    throw std::invalid_argument("Invalid X value");
  }
  return result;
}

/**
 * @brief Проверка корректности первой лексемы в очереди.
 */
void PolishNotation::ValidateFirstLexema() {
  auto first_lexema = parsed_lexemas_.front();
  auto first_group = first_lexema.GetGroup();
  auto first_type = first_lexema.GetType();
  if (first_group == g_closing_br ||
      (first_group == g_binary_op && first_type != t_minus &&
       first_type != t_plus)) {
    throw std::invalid_argument("Incorrect input");
  }
}

/**
 * @brief Проверка корректности последней лексемы в очереди.
 */
void PolishNotation::ValidateLastLexema() {
  auto last_lexema = parsed_lexemas_.back();
  auto last_group = last_lexema.GetGroup();
  if (last_group == g_binary_op || last_group == g_opening_br ||
      last_group == g_function) {
    throw std::invalid_argument("Incorrect input");
  }
}

/**
 * @brief Проверка корректности последовательности всех лексем.
 * @param brackets_count Количество встреченных в выражении скобок (и
 * открывающих, и закрывающих).
 */
void PolishNotation::ValidateParsedLexemas(int brackets_count) {
  ValidateFirstLexema();
  ValidateLastLexema();
  if (brackets_count != 0) {
    throw std::invalid_argument("Incorrect use of brackets");
  }

  std::deque<Lexema>::iterator lex_iter = parsed_lexemas_.begin();
  for (; lex_iter + 1 != parsed_lexemas_.end(); ++lex_iter) {
    if (!ValidityMatrix[lex_iter->GetGroup()][(lex_iter + 1)->GetGroup()]) {
      throw std::invalid_argument("Incorrect input");
    }
  }
}

/**
 * @brief Цикл для обработки лексем из очереди после парсинга.
 * @details Лексемы по одной извлекаются из начала очереди parsed_lexemas_ и
 * складываются по стекам (stack_of_numbers_ для чисел и stack_of_operators_ для
 * стального). Если приоритет лексемы на верхушке стэка stack_of_operators_ выше
 * приоритета текущей (вытолкнутой из очереди) лексемы, лежащие в стэке лексемы
 * будут выталкиваться и будут производиться соответствующие вычисления.
 */
void PolishNotation::LexemasProcessing() {
  while (!parsed_lexemas_.empty()) {
    Lexema current_lexema = parsed_lexemas_.front();
    parsed_lexemas_.pop_front();

    if (current_lexema.GetGroup() == g_number) {
      stack_of_numbers_.push(current_lexema);
    } else if (current_lexema.GetGroup() == g_closing_br) {
      ClosingBracketProcessing();
    } else if (current_lexema.GetGroup() == g_opening_br) {
      stack_of_operators_.push(current_lexema);
    } else if (stack_of_operators_.empty() ||
               current_lexema.GetPrioroty() >
                   stack_of_operators_.top().GetPrioroty()) {
      stack_of_operators_.push(current_lexema);
    } else {
      while (!stack_of_operators_.empty() &&
             current_lexema.GetPrioroty() <=
                 stack_of_operators_.top().GetPrioroty()) {
        Lexema prev_lex = GetOperationFromStack();
        MakeOperation(prev_lex);
      }
      stack_of_operators_.push(current_lexema);
    }
  }
}

/**
 * @brief Извлечение числа с верхушки стэка с числами stack_of_numbers_.
 * @return Число.
 */
double PolishNotation::GetNumberFromStack() {
  if (stack_of_numbers_.empty()) {
    throw std::invalid_argument("Incorrect input");
  }
  auto top_lexema = stack_of_numbers_.top();
  stack_of_numbers_.pop();

  double number;
  if (top_lexema.GetType() == t_number) {
    number = std::get<double>(top_lexema.GetFunction());
  } else if (top_lexema.GetType() == t_x) {
    number = x;
  }
  return number;
}

/**
 * @brief Извлечение лексемы с верхушки стэка с не числами stack_of_operators_
 * (функциями, операторами, скобками).
 * @return Лексема.
 */
PolishNotation::Lexema PolishNotation::GetOperationFromStack() {
  if (stack_of_operators_.empty()) {
    throw std::invalid_argument("Incorrect input");
  }
  Lexema lex = stack_of_operators_.top();
  stack_of_operators_.pop();
  return lex;
}

/**
 * @brief Осуществление математической операции, соответствующей текущей
 * лексеме.
 * @param lex Текущая лексема.
 */
void PolishNotation::MakeOperation(Lexema &lex) {
  double result;
  if (lex.GetGroup() == g_function) {
    double arg = GetNumberFromStack();
    auto func = std::get<unary_function>(lex.GetFunction());
    result = func(arg);
  } else if (lex.GetGroup() == g_binary_op) {
    double right_arg = GetNumberFromStack();
    double left_arg = GetNumberFromStack();
    auto func = std::get<binary_function>(lex.GetFunction());
    result = func(left_arg, right_arg);
  }
  stack_of_numbers_.push(
      Lexema(std::to_string(result), t_number, g_number, 0, result));
}

/**
 * @brief Действия при встрече закрывающй скобки в стэке.
 * @details Выталкиваются операторы и производятся соответствующие вычисления,
 * пока не будет встречена открывающая скобка.
 */
void PolishNotation::ClosingBracketProcessing() {
  int open_bracket_check = 0;
  while (!stack_of_operators_.empty()) {
    Lexema lex = GetOperationFromStack();
    if (lex.GetGroup() == g_opening_br) {
      open_bracket_check = 1;
      break;
    }
    MakeOperation(lex);
  }
  if (!open_bracket_check) {
    throw std::invalid_argument("Incorrect use of brackets");
  }
}

/**
 * @brief Цикл для финального подсчета (производятся операции оставшихся в
 * стэке лексем).
 */
void PolishNotation::FinalCalculations() {
  while (!stack_of_operators_.empty()) {
    Lexema current_lexema = GetOperationFromStack();
    MakeOperation(current_lexema);
  }
  if (stack_of_operators_.size() == 0 && stack_of_numbers_.size() == 1) {
    final_answer = GetNumberFromStack();
  } else {
    throw std::invalid_argument("Incorrect string");
  }
}

/**
 * @brief Вычисление значений для построения графика.
 * @param input_expression Строка с выражением.
 * @param x_min Нижняя граница области определения графика.
 * @param x_max Верхняя граница области определения графика.
 * @param x_data Вектор для значений X.
 * @param y_data Вектор для значений Y.
 * @throw std::invalid_argument В случае некорректности строки.
 */
void PolishNotation::GetGraph(std::string &input_expression, double x_min,
                              double x_max, std::vector<double> &x_data,
                              std::vector<double> &y_data) {
  if (x_max <= x_min) {
    throw std::invalid_argument("Incorrect borders");
  }
  double step = (x_max - x_min) / 499;
  try {
    ToLowerCase(input_expression);
    for (x = x_min; x <= x_max; x += step) {
      ParseExpression(input_expression);
      LexemasProcessing();
      FinalCalculations();
      x_data.push_back(x);
      y_data.push_back(GetAnswer());
    }
  } catch (const std::invalid_argument &ex) {
    ClearAll();
    throw;
  }
}

/**
 * @brief Очищает стэки и очереди (на случай некорректного завершения
 * вычисления).
 */
void PolishNotation::ClearAll() {
  x_is_found = false;
  parsed_lexemas_.clear();
  std::stack<Lexema>().swap(stack_of_numbers_);
  std::stack<Lexema>().swap(stack_of_operators_);
}

}  // namespace s21