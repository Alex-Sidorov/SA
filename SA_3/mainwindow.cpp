#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
void fill_graph_histogram(QSharedPointer<QCustomPlot> &graph, QVector<qreal> &data)
{   
    graph->setMinimumSize(400,500);
    auto max_value = QtConcurrent::run([&]
    {
        qreal result = 0;
        foreach(auto item,data)
        {
            if(result<item)
            {
                result = item;
            }
        }
        return result;
    });

    QPen pen;
    pen.setWidthF(1.5);
    pen.setColor(QColor(50, 50, 100));
    graph->graph(0)->setPen(pen);
    graph->graph(0)->setBrush(QColor(50, 50, 250, 70));

    QVector<qreal> ticks(256/8);
    for(int i = 4, j = 0; i <256; i+=8, j++)
    {
        ticks[j] = i;
    }

    auto scene = new QCPBars(graph->xAxis, graph->yAxis);
    scene->addData(ticks,data);
    scene->setWidth(8);

    QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
    fixedTicker->setTickStep(8);

    graph->xAxis->setTicker(fixedTicker);
    graph->xAxis->setTickLength(0, 10);
    graph->xAxis->grid()->setVisible(true);
    graph->xAxis->setRange(0, 256);

    graph->yAxis->setRange(0,max_value.result()+10000);
    graph->yAxis->setPadding(2);
    graph->yAxis->grid()->setSubGridVisible(true);

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    graph->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    graph->yAxis->grid()->setSubGridPen(gridPen);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _first(new QCustomPlot),
    _second(new QCustomPlot)
{
    ui->setupUi(this);

    QImage first;
    QImage second;
    if(!first.load("D:\\projectQt\\SA_3\\first.jpg") || !second.load("D:\\projectQt\\SA_3\\second.jpg"))
    {
        qDebug()<<"false";
        return;
    }

    ui->histograms->addWidget(_first.get());
    ui->histograms->addWidget(_second.get());

    auto first_grayscale = to_grayscale(first);
    auto second_grayscale = to_grayscale(second);
    auto first_histogram_values = create_interval_histogram(first_grayscale);
    auto second_histogram_values = create_interval_histogram(second_grayscale);
    QVector<qreal> first_vector_histogram_value(first_histogram_values.count());
    QVector<qreal> second_vector_histogram_value(second_histogram_values.count());
    auto iterator = second_vector_histogram_value.begin();
    std::transform(first_histogram_values.begin(),first_histogram_values.end(),
                   second_histogram_values.begin(),first_vector_histogram_value.begin(),
                   [&iterator](int &first_value,int &second_value)
    {
        (*iterator) = second_value;
        iterator++;
        return first_value;
    });

    ui->expected_one->setText("Мат. ожидание:" + QString::number(expected_value(first_grayscale)));
    ui->expected_two->setText("Мат. ожидание:" + QString::number(expected_value(second_grayscale)));
    /*auto interval_mode = mode_value(first_histogram_values);
    ui->mode_one->setText("Мода:" + QString::number(interval_mode.first) + "-" + QString::number(interval_mode.second));
    interval_mode = mode_value(second_histogram_values);
    ui->mode_two->setText("Мода:" + QString::number(interval_mode.first) + "-" + QString::number(interval_mode.second));*/
    ui->mode_one->setText("Мода:" + QString::number(mode_value(first_histogram_values,first_grayscale)));
    ui->mode_two->setText("Мода:" + QString::number(mode_value(second_histogram_values,second_grayscale)));
    ui->sdv_one->setText("СКО:" + QString::number(standart_deviation_value(first_grayscale),'f'));
    ui->sdv_two->setText("СКО:" + QString::number(standart_deviation_value(second_grayscale),'f'));
    ui->median_one->setText("Медиана:" + QString::number(median_value(first_histogram_values),'f'));
    ui->median_two->setText("Медиана:" + QString::number(median_value(second_histogram_values),'f'));
    ui->pirson_one->setText("Проверка гипотезы о нормальном распределении(Пирсон):" + (is_Pirson(first_histogram_values) ? QString("Принята") : QString("Опровергнута")));
    ui->pirson_two->setText("Проверка гипотезы о нормальном распределении(Пирсон):" + (is_Pirson(second_histogram_values) ? QString("Принята") : QString("Опровергнута")));
    ui->kolmagorov_one->setText("Проверка гипотезы о нормальном распределении(Колмагоров):" + (is_Kolmagorov(first_histogram_values) ? QString("Принята") : QString("Опровергнута")));
    ui->kolmagorov_two->setText("Проверка гипотезы о нормальном распределении(Колмагоров):" + (is_Kolmagorov(second_histogram_values) ? QString("Принята") : QString("Опровергнута")));
    ui->correlation_image->setText("Коэфицент корреляции для изображений:"+QString::number(correlation_value(first,second)));
    ui->correlation_histogram->setText("Коэфицент корреляции для гистограмм:"+QString::number(correlation_value(first_grayscale,second_grayscale)));

    _first->addGraph();
    fill_graph_histogram(_first,first_vector_histogram_value);
    _first->replot();

    _second->addGraph();
    fill_graph_histogram(_second,second_vector_histogram_value);
    _second->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
