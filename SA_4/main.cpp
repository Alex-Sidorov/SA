#include <QtDataVisualization/QtDataVisualization>
#include <QVector3D>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include "mainwindow.h"
#include <QPushButton>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    QVector<QPair<QString,int>> first;
    QVector<QPair<QString,int>> second;
    QVector<QPair<QString,int>> third;
    if(!create_bug_words(1,COUNT_LEARN_TEXT,SIZE_BUG_WORDS,first)
            || !create_bug_words(2,COUNT_LEARN_TEXT,SIZE_BUG_WORDS,second)
            || !create_bug_words(3,COUNT_LEARN_TEXT,SIZE_BUG_WORDS,third))
    {
        return -1;
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
        return -1;
    }

    auto first_data = get_coordinates(first_points_x,first_points_y,first_points_z);
    auto second_data = get_coordinates(second_points_x,second_points_y,second_points_z);
    auto third_data = get_coordinates(third_points_x,third_points_y,third_points_z);

    auto temp_data = first_data.mid(0,4) + second_data.mid(0,4) + third_data.mid(0,4);
    decltype (temp_data) centre(3);
    centre[0] = temp_data[2];
    centre[1] = temp_data[6];
    centre[2] = temp_data[11];
    clasterization(temp_data,centre);

    temp_data = first_data + second_data + third_data;
    auto result = create_result_clastarization(temp_data,centre);
//////////////////////////////

    QtDataVisualization::QScatterDataArray centre_first = {QVector3D(centre[0].x,centre[0].y,centre[0].z)};
    QtDataVisualization::QScatterDataArray centre_second = {QVector3D(centre[1].x,centre[1].y,centre[1].z)};
    QtDataVisualization::QScatterDataArray centre_third = {QVector3D(centre[2].x,centre[2].y,centre[2].z)};

    QtDataVisualization::QScatter3DSeries *series_centre_first = new QtDataVisualization::QScatter3DSeries;
    QtDataVisualization::QScatter3DSeries *series_centre_second = new QtDataVisualization::QScatter3DSeries;
    QtDataVisualization::QScatter3DSeries *series_centre_third = new QtDataVisualization::QScatter3DSeries;

    QLinearGradient grad_centre_first(QPointF(100, 100), QPointF(200, 200));
    grad_centre_first.setColorAt(0, Qt::blue);
    grad_centre_first.setColorAt(1, Qt::white);

    QLinearGradient grad_centre_second(QPointF(100, 100), QPointF(200, 200));
    grad_centre_second.setColorAt(0, Qt::green);
    grad_centre_second.setColorAt(1, Qt::white);

    QLinearGradient grad_centre_third(QPointF(100, 100), QPointF(200, 200));
    grad_centre_third.setColorAt(0, Qt::red);
    grad_centre_third.setColorAt(1, Qt::white);

    QtDataVisualization::QScatterDataArray data_first;
    QtDataVisualization::QScatterDataArray data_second;
    QtDataVisualization::QScatterDataArray data_third;

    foreach(auto item,result)
    {
        switch (item.second)
        {
        case 1:data_first<<QVector3D(item.first.x,item.first.y,item.first.z);break;
        case 2:data_second<<QVector3D(item.first.x,item.first.y,item.first.z);break;
        case 3:data_third<<QVector3D(item.first.x,item.first.y,item.first.z);break;
        }
    }

    QtDataVisualization::Q3DScatter scatter;
    scatter.setFlags(scatter.flags() ^ Qt::FramelessWindowHint);

    QtDataVisualization::QScatter3DSeries *series_first = new QtDataVisualization::QScatter3DSeries;
    QtDataVisualization::QScatter3DSeries *series_second = new QtDataVisualization::QScatter3DSeries;
    QtDataVisualization::QScatter3DSeries *series_third = new QtDataVisualization::QScatter3DSeries;


    QLinearGradient grad_first(QPointF(100, 100), QPointF(200, 200));
    grad_first.setColorAt(0, Qt::blue);

    QLinearGradient grad_second(QPointF(100, 100), QPointF(200, 200));
    grad_second.setColorAt(0, Qt::green);

    QLinearGradient grad_third(QPointF(100, 100), QPointF(200, 200));
    grad_third.setColorAt(0, Qt::red);

    series_first->setBaseGradient(grad_first);
    series_first->setColorStyle(QtDataVisualization::Q3DTheme::ColorStyle::ColorStyleObjectGradient);
    series_first->dataProxy()->addItems(data_first);
    scatter.addSeries(series_first);

    series_second->setBaseGradient(grad_second);
    series_second->setColorStyle(QtDataVisualization::Q3DTheme::ColorStyle::ColorStyleObjectGradient);
    series_second->dataProxy()->addItems(data_second);
    scatter.addSeries(series_second);

    series_third->setBaseGradient(grad_third);
    series_third->setColorStyle(QtDataVisualization::Q3DTheme::ColorStyle::ColorStyleObjectGradient);
    series_third->dataProxy()->addItems(data_third);
    scatter.addSeries(series_third);

    series_centre_first->setBaseGradient(grad_centre_first);
    series_centre_first->setColorStyle(QtDataVisualization::Q3DTheme::ColorStyle::ColorStyleObjectGradient);
    series_centre_first->dataProxy()->addItems(centre_first);
    scatter.addSeries(series_centre_first);

    series_centre_second->setBaseGradient(grad_centre_second);
    series_centre_second->setColorStyle(QtDataVisualization::Q3DTheme::ColorStyle::ColorStyleObjectGradient);
    series_centre_second->dataProxy()->addItems(centre_second);
    scatter.addSeries(series_centre_second);

    series_centre_third->setBaseGradient(grad_centre_third);
    series_centre_third->setColorStyle(QtDataVisualization::Q3DTheme::ColorStyle::ColorStyleObjectGradient);
    series_centre_third->dataProxy()->addItems(centre_third);
    scatter.addSeries(series_centre_third);

    int preset = QtDataVisualization::Q3DCamera::CameraPresetRightHigh;
    scatter.scene()->activeCamera()->setCameraPreset((QtDataVisualization::Q3DCamera::CameraPreset)preset);

    QWidget *container = QWidget::createWindowContainer(&scatter);
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);

    QFrame line;
    line.setFrameShape(QFrame::VLine);
    line.setFrameShadow(QFrame::Sunken);
    hLayout->addWidget(&line,0);

    hLayout->addLayout(vLayout);

    QListWidget list;
    vLayout->addWidget(&list);

    QPushButton up("up");
    QPushButton down("down");
    QPushButton right("right");
    QPushButton left("left");

    up.connect(&up,&QPushButton::clicked,[&scatter]{
        auto y = scatter.scene()->activeCamera()->yRotation();
        scatter.scene()->activeCamera()->setYRotation(y + 2);
    });
    down.connect(&down,&QPushButton::clicked,[&scatter]{
        auto y = scatter.scene()->activeCamera()->yRotation();
        scatter.scene()->activeCamera()->setYRotation(y - 2);
    });
    left.connect(&left,&QPushButton::clicked,[&scatter]{
        auto x = scatter.scene()->activeCamera()->xRotation();
        scatter.scene()->activeCamera()->setXRotation(x - 2);
    });
    right.connect(&right,&QPushButton::clicked,[&scatter]{
        auto x = scatter.scene()->activeCamera()->xRotation();
        scatter.scene()->activeCamera()->setXRotation(x + 2);
    });

    vLayout->addWidget(&up);
    vLayout->addWidget(&down);
    vLayout->addWidget(&right);
    vLayout->addWidget(&left);

    for(auto i = 0; i < COUNT_TEXT_THEME*COUNT_THEME;i++)
    {
        auto temp = QString::number(result[i].first.x)
                + " ; "
                +QString::number(result[i].first.y)
                + " ; "
                +QString::number(result[i].first.z)
                + " | "
                +QString::number(result[i].second)
                + " | ";
        if(i<COUNT_TEXT_THEME)
        {
            temp+="1";

        }
        else if(i<COUNT_TEXT_THEME*(COUNT_THEME-1))
        {
            temp+="2";
        }
        else
        {
            temp+="3";
        }
        list.addItem(temp);
    }
    //scatter.show();
    widget->show();
    return a.exec();
}
