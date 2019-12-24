#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "parser.h"
#include "qcustomplot.h"
#include "function.h"

#include <QMainWindow>
#include <QSharedPointer>
#include <QPair>
#include <QFuture>
#include <QtConcurrent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slot_get_result_parser();

private:
    Ui::MainWindow *ui;
    Parser _parser;
    QSharedPointer<QCustomPlot> _scene;
};

#endif // MAINWINDOW_H
