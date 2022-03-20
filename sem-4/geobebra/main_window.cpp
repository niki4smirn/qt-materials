#include "main_window.h"

MainWindow::MainWindow()
    : widget_(new QWidget(this)),
      layout_(new QGridLayout()),
      paint_widget_(new PaintWidget(widget_)),
      line_edit_(new QLineEdit(widget_)),
      add_button_(new QPushButton(widget_)),
      delete_button_(new QPushButton(widget_)) {
  resize(minimal_size_);
  setMinimumSize(minimal_size_);

  add_button_->setText("Add");
  delete_button_->setText("Delete");

  RefreshCurPlot();

  ConnectWidgets();

  layout_->setRowStretch(0, 10);
  layout_->setRowStretch(1, 1);

  layout_->addWidget(paint_widget_, 0, 0, 1, 2);
  layout_->addWidget(add_button_, 1, 0);
  layout_->addWidget(delete_button_, 1, 1);
  layout_->addWidget(line_edit_, 2, 0, 1, 2);

  widget_->setLayout(layout_);

  setCentralWidget(widget_);
}

void MainWindow::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);
  QPainter painter(this);

  RefreshCurPlot();
  paint_widget_->Paint(&painter, cur_plot_);
}

void MainWindow::ConnectWidgets() {
  connect(line_edit_,
          &QLineEdit::editingFinished, [&]() {
        RefreshCurPlot();
        repaint();
      });

  connect(add_button_,
          &QPushButton::clicked, [&]() {});

  connect(delete_button_,
          &QPushButton::clicked, [&]() {});
}

void MainWindow::RefreshCurPlot() {
  QStringList str_parameters = line_edit_->text().split(", ");
  std::vector<double> parameters;
  for (const auto& str_parameter : str_parameters) {
    parameters.push_back(str_parameter.toDouble());
  }

  int width = paint_widget_->width();
  int height = paint_widget_->height();

  cur_plot_.clear();
  cur_plot_.reserve(width);

  for (int x = -width / 2; x < width / 2; ++x) {
    double y = 0;
    for (double cur_parameter : parameters) {
      y *= x;
      y += cur_parameter;
    }
    if (y > -height / 2 && y < height / 2) {
      cur_plot_.emplace_back(x, static_cast<int>(-y));
    }
  }
}
