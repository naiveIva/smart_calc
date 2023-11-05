#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "../controller/controller.h"
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief Класс представления для графического интерфейса.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);

  ~MainWindow();

  std::string GetInputString() const;

  std::string GetXString() const;

  void SetAnswer(double answer);

 signals:
  /**
   * @brief Сигнал при нажатии на кнопу build. Вызывает функцию для построения
   * графика в классе graph.
   * @param input_expr Строка с выражением для вычисления.
   * @param x_min Минимальное значение X для графика.
   * @param x_max Максимальное значение X для графика.
   * @param y_min Минимальное значение Y для графика.
   * @param y_max Максимальное значение Y для графика.
   */
  void build(std::string &input_expr, double x_min, double x_max, double y_min,
             double y_max);

 private:
  Ui::MainWindow *ui;
  s21::Controller controller_;  ///< контроллер
  graph *graph_window;  ///< Отдельное окно для графика

 private slots:
  void SignalsSetUp();

  void PushButton();

  void PushWithBracket();

  void DeleteLexema();

  void ClearInputStr();

  void on_pushButton_eq_clicked();

  void on_pushButton_build_3_clicked();
};

#endif  // MAINWINDOW_H
