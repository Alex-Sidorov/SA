#ifndef FUNCTION_H
#define FUNCTION_H

#include <QImage>
#include <QVector>
#include <QRgb>
#include <QMap>
#include <math.h>
#include <QtConcurrent>
#include <QFuture>
#include <QtDebug>

constexpr auto INTERVAL = 8;
constexpr auto CRITICAL_VALUE = 42.557;
constexpr auto LAMBDA_05 = 1.36;

enum VALUES{PLUS_INFINITY = 1, MINUS_INFINITY,VALUE};

QVector<QRgb> to_grayscale(QImage &image);
qreal expected_value(QVector<QRgb> &data);
qreal expected_value(QVector<int> &data);
QPair<int,int> mode_value(QMap<QPair<int,int>,int> &map);
int mode_value(QMap<QPair<int,int>,int> &map,QVector<QRgb> &data);
qreal median_value(QMap<QPair<int,int>,int> &data);
qreal standart_deviation_value(QVector<QRgb> &data);
qreal correlation_value(QImage &first, QImage &second);
qreal correlation_value(QVector<QRgb> &first, QVector<QRgb> &second);
QMap<QPair<int,int>,int> create_interval_histogram(QVector<QRgb> &grayscale_image);
//int get_count_values_interval(QMap<qreal,int> &histogram,QPair<int,int> &&range);
int get_count_values_interval(QVector<QRgb> &grayscale, QSet<QRgb> &intensiv,QPair<int,int> &&range);

qreal find_nominator_correlation_histogram(QVector<int> &x, QVector<int> &y);
qreal find_denominator_correlation_histogram(QVector<int> &x, QVector<int> &y);
qreal find_nominator_correlation_image(QVector<QRgb> &x, QVector<QRgb> &y);
qreal find_denominator_correlation_image(QVector<QRgb> &x, QVector<QRgb> &y);

bool is_Pirson(QMap<QPair<int,int>,int> &map);
qreal expected_value(QVector<QPair<int,qreal>> &data);
qreal dispersion_value(QVector<QPair<int,qreal>> &data);
qreal norm_fun(qreal value,VALUES flag);
qreal teoretic_m_value(int count, qreal sdv,qreal expected,QPair<int,int> &range);
bool is_Kolmagorov(QMap<QPair<int,int>,int> &histogram);



#endif // FUNCTION_H
