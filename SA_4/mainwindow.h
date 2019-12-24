#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "function.h"
#include <QtDataVisualization/Q3DScatter>
#include <QtDataVisualization/QtDataVisualization>
#include <QVector3D>
#include <QtDataVisualization/qabstract3dseries.h>

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
    QtDataVisualization::Q3DScatter _scatter;
    QtDataVisualization::QScatterDataArray _data;
};

#endif // MAINWINDOW_H
