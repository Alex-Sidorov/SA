#ifndef FUNCTION_H
#define FUNCTION_H

#include <qmath.h>
#include <QVector>
#include <QByteArray>
#include <QFuture>
#include <QtConcurrent>

qreal expected_value(QVector<QByteArray> &values);
qreal correlation_value(QVector<qreal> &x, QVector<qreal> &y);
qreal find_nominator_correlation(QVector<qreal> &x, QVector<qreal> &y);
qreal find_denominator_correlation(QVector<qreal> &x, QVector<qreal> &y);
qreal find_a(QVector<qreal> &x, QVector<qreal> &y);
qreal find_nominator_function_a(QVector<qreal> &x, QVector<qreal> &y);
qreal find_b(QVector<qreal> &x, QVector<qreal> &y);
qreal find_nominator_function_b(QVector<qreal> &x, QVector<qreal> &y);
qreal find_denominator_function(QVector<qreal> &x);
qreal get_point_y(qreal x, qreal a, qreal b);

#endif // FUNCTION_H
