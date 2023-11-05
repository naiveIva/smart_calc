#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  SignalsSetUp();
  s21::Controller *controller_ptr = &controller_;
  graph_window = new graph();
  graph_window->SetController(controller_ptr);
  connect(this, &MainWindow::build, graph_window, &graph::build);
}

MainWindow::~MainWindow() { delete ui; }

/* Геттер, возвращающий строку с введенным выражением для вычисления. */
std::string MainWindow::GetInputString() const {
  return ui->lineEdit_input->text().toStdString();
}

/* Геттер, возвращающий строку с введенным значением икса. */
std::string MainWindow::GetXString() const {
  return ui->lineEdit_x_input->text().toStdString();
}

/* Сеттер, задающий вычисленный ответ. */
void MainWindow::SetAnswer(double answer) {
  ui->label_result->setText(QString::number(answer, 'g', 10));
}

/* Соединение всех сигналов от нажатия кнопок со слотами. */
void MainWindow::SignalsSetUp() {
  connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_point, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_e, SIGNAL(clicked()), this, SLOT(PushButton()));

  connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_mul, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(PushButton()));

  connect(ui->pushButton_br1, SIGNAL(clicked()), this, SLOT(PushButton()));
  connect(ui->pushButton_br2, SIGNAL(clicked()), this, SLOT(PushButton()));

  connect(ui->pushButton_sqrt, SIGNAL(clicked()), this,
          SLOT(PushWithBracket()));
  connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(PushWithBracket()));
  connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(PushWithBracket()));
  connect(ui->pushButton_acos, SIGNAL(clicked()), this,
          SLOT(PushWithBracket()));
  connect(ui->pushButton_asin, SIGNAL(clicked()), this,
          SLOT(PushWithBracket()));
  connect(ui->pushButton_atan, SIGNAL(clicked()), this,
          SLOT(PushWithBracket()));
  connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(PushWithBracket()));
  connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(PushWithBracket()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(PushWithBracket()));

  connect(ui->pushButton_x, SIGNAL(clicked()), this, SLOT(PushButton()));

  connect(ui->pushButton_AC, SIGNAL(clicked()), this, SLOT(ClearInputStr()));
  connect(ui->pushButton_del, SIGNAL(clicked()), this, SLOT(DeleteLexema()));
}

/* Добавляет текст с нажатой кнопки к вводимому выражению. */
void MainWindow::PushButton() {
  QPushButton *button = (QPushButton *)sender();
  QString full_str;
  full_str = ui->lineEdit_input->text() + button->text();
  ui->lineEdit_input->setText(full_str);
}

/* Добавляет текст с нажатой кнопки и открывающую скобку к вводимому выражению.
 */
void MainWindow::PushWithBracket() {
  QPushButton *button = (QPushButton *)sender();
  QString full_str;
  full_str = ui->lineEdit_input->text() + button->text() + "(";
  ui->lineEdit_input->setText(full_str);
}

/* Удаляет последнюю введнную лексему из выражения. */
void MainWindow::DeleteLexema() {
  QString str_copy = ui->lineEdit_input->text();
  QString new_str;
  size_t len = str_copy.length();
  if (len != 0) {
    if (str_copy.right(4) == "asin" || str_copy.right(4) == "acos" ||
        str_copy.right(4) == "atan" || str_copy.right(4) == "sqrt") {
      new_str = str_copy.left(len - 4);
    } else if (str_copy.right(3) == "sin" || str_copy.right(3) == "cos" ||
               str_copy.right(3) == "tan" || str_copy.right(3) == "log" ||
               str_copy.right(3) == "mod") {
      new_str = str_copy.left(len - 3);
    } else if (str_copy.right(2) == "ln") {
      new_str = str_copy.left(len - 2);
    } else {
      new_str = str_copy.left(len - 1);
    }
    ui->lineEdit_input->setText(new_str);
  }
}

/* Очищает строку с выражением. */
void MainWindow::ClearInputStr() {
  ui->lineEdit_input->setText("");
  ui->lineEdit_x_input->setText("");
  ui->label_result->setText("");
}

/* Действия при нажатии на кнопку "равно": обращение к контроллеру и вычисление.
 */
void MainWindow::on_pushButton_eq_clicked() {
  std::string input_str = GetInputString();
  std::string x_str = GetXString();
  try {
    double answer = controller_.CalculateValue(input_str, x_str);
    SetAnswer(answer);
  } catch (const std::exception &ex) {
    QMessageBox::warning(this, "Error", ex.what());
    input_str = "";
    x_str = "";
  }
}

/* Действия при нажатии на кнопку "build": обращение к контроллеру и построение
 * графика.*/
void MainWindow::on_pushButton_build_3_clicked() {
  bool x_min_check, x_max_check, y_min_check, y_max_check;
  double x_min = ui->lineEdit_x_min->text().toDouble(&x_min_check);
  double x_max = ui->lineEdit_x_max->text().toDouble(&x_max_check);
  double y_min = ui->lineEdit_y_min->text().toDouble(&y_min_check);
  double y_max = ui->lineEdit_y_max->text().toDouble(&y_max_check);
  std::string input_str = GetInputString();

  if (!(x_min_check && x_max_check && y_min_check && y_max_check) ||
      x_min >= x_max || y_min >= y_max) {
    QMessageBox::warning(this, "Error", "Incorrect borders for graph");
  } else {
    try {
      emit build(input_str, x_min, x_max, y_min, y_max);
      graph_window->show();
    } catch (const std::exception &ex) {
    }
  }
}
