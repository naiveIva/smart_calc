#ifndef GRAPH_H
#define GRAPH_H

#include <QMainWindow>

#include "../controller/controller.h"
#include "qcustomplot.h"

namespace Ui {
class graph;
}

/**
 * @brief Класс для отдельного окна с графиком.
 */
class graph : public QMainWindow {
  Q_OBJECT

 public:
  explicit graph(QWidget *parent = nullptr);

  ~graph();

  void SetController(s21::Controller *controller);

 public slots:

  void build(std::string &input_expr, double x_min, double x_max, double y_min,
             double y_max);

 private:
  Ui::graph *ui;
  s21::Controller *controller_;  ///< Контроллер.
};

#endif  // GRAPH_H
