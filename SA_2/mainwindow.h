#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "parser.h"
#include "function.h"

#include <QMainWindow>
#include <QTableWidgetItem>

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

private slots:
    void on_update_clicked();

private:
    Ui::MainWindow *ui;
    Parser _parser;
    QVector<DataFile> _data;
};

#endif // MAINWINDOW_H
