#ifndef FUNCTION_H
#define FUNCTION_H

#include <qmath.h>
#include <QVector>
#include <QByteArray>
#include <QFuture>
#include <QtConcurrent>

constexpr auto T_CRITERION_95_150 = 1.97603;
constexpr auto T_CRITERION_95_300 = 1.96792;
constexpr auto X2_VALUE_DOWN = 179.5806;
constexpr auto X2_VALUE_UP = 117.9845;

qreal expected_value(QVector<qreal> &values);
qreal dispersion_value(QVector<qreal> &values);
qreal criterion_z_value(QVector<qreal> &values_one,QVector<qreal> &values_two);
qreal criterion_t_value(QVector<qreal> &values_one,QVector<qreal> &values_two);

#endif // FUNCTION_H
