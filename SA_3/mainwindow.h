#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include "qcustomplot.h"
#include "function.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSharedPointer<QCustomPlot> _first;
    QSharedPointer<QCustomPlot> _second;
};

#endif // MAINWINDOW_H
