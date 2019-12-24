#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&_parser,&Parser::downloaded,this,&MainWindow::slot_get_result_parser);
    QUrl url("https://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data");
    _parser.set_url(url);
    _parser.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_get_result_parser()
{
    _parser.parsing();
    _data = _parser.get_parsed_data();
    QString temp;
    for(int i = 0; i <_data.size(); i++)
    {
        ui->table->insertRow(ui->table->rowCount());
        temp = QString::fromUtf8(_data[i].col_one);
        ui->table->setItem(i,0, new QTableWidgetItem(temp));
        temp = QString::fromUtf8(_data[i].col_two);
        ui->table->setItem(i,1, new QTableWidgetItem(temp));
        temp = QString::fromUtf8(_data[i].col_three);
        ui->table->setItem(i,2, new QTableWidgetItem(temp));
        temp = QString::fromUtf8(_data[i].col_four);
        ui->table->setItem(i,3, new QTableWidgetItem(temp));
    }
    ui->update->setEnabled(true);
}

void create_vector(QVector<qreal> &result,QVector<DataFile> &data, int col)
{
    switch (col)
    {
    case 0:
        foreach(auto item,data)
        {
            result.push_back(item.col_one.toDouble());
        }
        break;
    case 1:
        foreach(auto item,data)
        {
            result.push_back(item.col_two.toDouble());
        }
        break;
    case 2:
        foreach(auto item,data)
        {
            result.push_back(item.col_three.toDouble());
        }
        break;
    case 3:
        foreach(auto item,data)
        {
            result.push_back(item.col_four.toDouble());
        }
    }
}

void MainWindow::on_update_clicked()
{
    QVector<qreal> first_values;
    QVector<qreal> second_values;
    create_vector(first_values,_data,ui->col_one->currentIndex());
    create_vector(second_values,_data,ui->col_two->currentIndex());

    auto expected_one = QtConcurrent::run(expected_value,first_values);
    auto dispersion_one = dispersion_value(first_values);
    expected_one.waitForFinished();
    ui->expected_label->setText("Математическое ожидание:"+QString::number(expected_one.result()));
    ui->dispersion_label->setText("Дисперсия:"+QString::number(dispersion_one));
    ui->interval_expected_label->setText("Доверительный интервал для математического ожидания:"+
                                QString::number(T_CRITERION_95_150*sqrt(dispersion_one/first_values.count())));
    ui->interval_dispersion_label->setText("Доверительный интервал для дисперсии:("+
                                QString::number((dispersion_one*(first_values.count()-1))/X2_VALUE_DOWN)+
                                           ';' + QString::number((dispersion_one*(first_values.count()-1))/X2_VALUE_UP) + ')');

    auto t_criterion = QtConcurrent::run(criterion_t_value,first_values,second_values);
    auto z_criterion = criterion_z_value(first_values,second_values);
    ui->z_criterion->setText("Z-критерий:"+QString::number(z_criterion));
    t_criterion.waitForFinished();
    ui->t_criterion->setText("T-критерий:"+QString::number(t_criterion.result()));

    QString result_hypothesis = t_criterion.result()<T_CRITERION_95_300?"принята":"опровергнута";
    ui->hypothesis_know_d->setText("Гипотеза о равенстве математических ожиданий(дисперсии известны):" + result_hypothesis);
    result_hypothesis = z_criterion<T_CRITERION_95_300?"принята":"опровергнута";
    ui->hypothesis_not_know_d->setText("Гипотеза о равенстве математических ожиданий(дисперсии неизвестны):" + result_hypothesis);

}
