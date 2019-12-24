#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*auto result = result_clastarization();
    if(result.isEmpty())
    {
        return;
    }*/
    /*QVector<QPair<QString,int>> first;
    QVector<QPair<QString,int>> second;
    QVector<QPair<QString,int>> third;
    if(!create_bug_words(1,4,60,first)
            || !create_bug_words(2,4,60,second)
            || !create_bug_words(3,4,60,third))
    {
        return;
    }

    foreach(auto i, first)
    {
        qDebug()<<i;
    }
    foreach(auto i, second)
    {
        qDebug()<<i;
    }
    foreach(auto i, third)
    {
        qDebug()<<i;
    }

    QVector<int> first_points_x(COUNT_TEXT_THEME);
    QVector<int> first_points_y(COUNT_TEXT_THEME);
    QVector<int> first_points_z(COUNT_TEXT_THEME);
    QVector<int> second_points_x(COUNT_TEXT_THEME);
    QVector<int> second_points_y(COUNT_TEXT_THEME);
    QVector<int> second_points_z(COUNT_TEXT_THEME);
    QVector<int> third_points_x(COUNT_TEXT_THEME);
    QVector<int> third_points_y(COUNT_TEXT_THEME);
    QVector<int> third_points_z(COUNT_TEXT_THEME);

    if(!count_true_words_in_text(1,COUNT_TEXT_THEME,first,first_points_x)
            ||!count_true_words_in_text(1,COUNT_TEXT_THEME,second,first_points_y)
            ||!count_true_words_in_text(1,COUNT_TEXT_THEME,third,first_points_z)
            ||!count_true_words_in_text(2,COUNT_TEXT_THEME,first,second_points_x)
            ||!count_true_words_in_text(2,COUNT_TEXT_THEME,second,second_points_y)
            ||!count_true_words_in_text(2,COUNT_TEXT_THEME,third,second_points_z)
            ||!count_true_words_in_text(3,COUNT_TEXT_THEME,first,third_points_x)
            ||!count_true_words_in_text(3,COUNT_TEXT_THEME,second,third_points_y)
            ||!count_true_words_in_text(3,COUNT_TEXT_THEME,third,third_points_z))
    {
        return ;
    }

    auto first_data = get_coordinates(first_points_x,first_points_y,first_points_z);
    auto second_data = get_coordinates(second_points_x,second_points_y,second_points_z);
    auto third_data = get_coordinates(third_points_x,third_points_y,third_points_z);

    auto temp_data = first_data.mid(0,4) + second_data.mid(0,4) + third_data.mid(0,4);
    decltype (temp_data) centre;
    centre[0] = temp_data[2];
    centre[1] = temp_data[6];
    centre[2] = temp_data[10];
    clasterization(temp_data,centre);

    temp_data = first_data + second_data + third_data;
    auto result = create_result_clastarization(temp_data,centre);*/

    /*_scatter.setFlags(_scatter.flags() ^ Qt::FramelessWindowHint);
    QtDataVisualization::QScatter3DSeries *series = new QtDataVisualization::QScatter3DSeries;
    _data << QVector3D(0.5f, 0.5f, 0.5f) << QVector3D(-0.3f, -0.5f, -4) << QVector3D(0.0f, -0.3f, 0.2f);
    series->dataProxy()->addItems(_data);
    _scatter.addSeries(series);
    _scatter.show();*/
}

MainWindow::~MainWindow()
{
    delete ui;
}
