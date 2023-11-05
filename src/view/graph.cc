#include "graph.h"

#include "./ui_graph.h"

graph::graph(QWidget *parent) : QMainWindow(parent), ui(new Ui::graph) {
  ui->setupUi(this);
}

graph::~graph() { delete ui; }

/**
 * @brief Функция-сеттер, задаёт контроллер.
 * @param controller Ссылка на контроллер.
 */
void graph::SetController(s21::Controller *controller) {
  controller_ = controller;
}

/**
 * @brief Функция (слот) для построения графика.
 * @param input_expr Строка с выражением для вычисления.
 * @param x_min Минимальное значение X для графика.
 * @param x_max Максимальное значение X для графика.
 * @param y_min Минимальное значение Y для графика.
 * @param y_max Максимальное значение Y для графика.
 */
void graph::build(std::string &input_expr, double x_min, double x_max,
                  double y_min, double y_max) {
  try {
    std::vector<double> x_vector;
    std::vector<double> y_vector;

    controller_->GetDataForGraph(input_expr, x_min, x_max, x_vector, y_vector);
    QVector<double> x_qvector =
        QVector<double>(x_vector.begin(), x_vector.end());
    QVector<double> y_qvector =
        QVector<double>(y_vector.begin(), y_vector.end());

    ui->widget_graph->clearGraphs();
    ui->widget_graph->addGraph();

    ui->widget_graph->xAxis->setRange(x_min, x_max);
    ui->widget_graph->yAxis->setRange(y_min, y_max);

    ui->widget_graph->graph(0)->setData(x_qvector, y_qvector);
    ui->widget_graph->replot();
  } catch (const std::exception &ex) {
    QMessageBox::warning(this, "Error", ex.what());
    throw;
  }
}
