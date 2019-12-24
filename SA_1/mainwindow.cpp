#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _scene(new QCustomPlot())
{
    qDebug()<<QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() ;
    ui->setupUi(this);
    connect(&_parser,&Parser::downloaded,this,&MainWindow::slot_get_result_parser);
    QUrl url("https://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data");
    _parser.set_url(url);
    _parser.start();
}

void MainWindow::slot_get_result_parser()
{
    _parser.parsing();
    auto data = _parser.get_parsed_data();
    QString temp;
    for(int i = 0; i <data.size(); i++)
    {
        temp = QString::fromUtf8(data[i].col_two);
        ui->table->insertRow(ui->table->rowCount());
        ui->table->setItem(i,0, new QTableWidgetItem(temp));
        temp = QString::fromUtf8(data[i].col_three);
        ui->table->setItem(i,1, new QTableWidgetItem(temp));
    }
    QVector<qreal> x;
    QVector<qreal> y;
    foreach(auto item,data)
    {
        x.push_back(item.col_two.toDouble());
        y.push_back(item.col_three.toDouble());
    }
    ui->correlation->setText(ui->correlation->text() + QString::number(correlation_value(x,y)));
    QFuture<QPair<qreal,qreal>> range_x = QtConcurrent::run([x]()
    {
        qreal min = x[0];
        qreal max = x[0];
        foreach(auto item, x)
        {
            if(max < item)
            {
                max = item;
            }
            if(min > item)
            {
                min = item;
            }
        }
        return QPair<qreal,qreal>(min,max);
    });
    QFuture<QPair<qreal,qreal>> range_y = QtConcurrent::run([y]()
    {
        qreal min = y[0];
        qreal max = y[0];
        foreach(auto item, y)
        {
            if(max < item)
            {
                max = item;
            }
            if(min > item)
            {
                min = item;
            }
        }
        return QPair<qreal,qreal>(min,max);
    });
    auto a = find_a(x,y);
    auto b = find_b(x,y);
    range_x.waitForFinished();

    QVector<qreal> points_x;
    QVector<qreal> points_y;
    for(qreal i = range_x.result().first; i <= range_x.result().second; i+=0.1)
    {
        points_x.push_back(i);
        points_y.push_back(get_point_y(i,a,b));
    }

    range_y.waitForFinished();
    ui->horizontalLayout->addWidget(_scene.get());
    _scene->setMinimumSize(range_x.result().second + 600,range_y.result().second + 500);

    _scene->addGraph();
    _scene->graph(0)->setData(points_x,points_y);

    _scene->addGraph(_scene->xAxis,_scene->yAxis);
    _scene->graph(1)->setPen(QColor(50, 50, 50, 255));
    _scene->graph(1)->setLineStyle(QCPGraph::lsNone);
    _scene->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
    _scene->graph(1)->setData(x,y);

    _scene->xAxis->setRange(0,range_x.result().second + 1);
    _scene->yAxis->setRange(0,range_y.result().second + 1);

    //_scene->rescaleAxes();
    _scene->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
