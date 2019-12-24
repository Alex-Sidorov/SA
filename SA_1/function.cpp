#include "function.h"

qreal expected_value(QVector<qreal> &values)
{
    qreal result = 0;
    foreach(auto item, values)
    {
        result+=item;
    }
    return result/values.size();
}

qreal find_nominator_correlation(QVector<qreal> &x, QVector<qreal> &y)
{
    auto expected_x = expected_value(x);
    auto expected_y = expected_value(y);
    qreal result = 0;
    for (int i = 0; i < x.size(); i++)
    {
        result += (x[i]-expected_x)*(y[i]-expected_y);
    }
    return result;
}

qreal find_denominator_correlation(QVector<qreal> &x, QVector<qreal> &y)
{
    auto expected_x = expected_value(x);
    auto expected_y = expected_value(y);
    qreal x_ = 0;
    qreal y_ = 0;
    for (int i = 0; i < x.size(); i++)
    {
        x_+= qPow(((x[i]-expected_x)),2);
        y_+= qPow(((y[i]-expected_y)),2);
    }
    return qSqrt(x_*y_);
}

qreal correlation_value(QVector<qreal> &x, QVector<qreal> &y)
{
    QFuture<qreal> nominator = QtConcurrent::run(find_nominator_correlation,x,y);
    QFuture<qreal> denominator = QtConcurrent::run(find_denominator_correlation,x,y);
    nominator.waitForFinished();
    denominator.waitForFinished();
    return nominator.result()/denominator.result();
}

qreal find_nominator_function_a(QVector<qreal> &x, QVector<qreal> &y)
{
    QFuture<qreal> one_value = QtConcurrent::run([x,y]()
    {
        qreal result = 0;
        for (int i = 0; i < x.size(); i++)
        {
           result+=x[i]*y[i];
        }
        return x.size()*result;
    });

    QFuture<qreal> two_value = QtConcurrent::run([x,y]()
    {
        qreal x_ = 0;
        qreal y_ = 0;
        for (int i = 0; i < x.size(); i++)
        {
           x_+=x[i];
           y_+=y[i];
        }
        return x_*y_;
    });

    one_value.waitForFinished();
    two_value.waitForFinished();
    return one_value.result()-two_value.result();
}

qreal find_nominator_function_b(QVector<qreal> &x, QVector<qreal> &y)
{
    QFuture<qreal> one_value = QtConcurrent::run([x,y]()
    {
        qreal x_ = 0;
        qreal y_ = 0;
        for (int i = 0; i < x.size(); i++)
        {
            x_+=qPow(x[i],2);
            y_+=y[i];
        }
        return x_*y_;
    });

    QFuture<qreal> two_value = QtConcurrent::run([x,y]()
    {
        qreal x_ = 0;
        qreal xy_ = 0;
        for (int i = 0; i < x.size(); i++)
        {
           x_+=x[i];
           xy_+=x[i]*y[i];
        }
        return x_*xy_;
    });

    one_value.waitForFinished();
    two_value.waitForFinished();
    return one_value.result()-two_value.result();
}

qreal find_denominator_function(QVector<qreal> &x)
{
    QFuture<qreal> one_value = QtConcurrent::run([x]()
    {
        qreal result = 0;
        for (int i = 0; i < x.size(); i++)
        {
           result+=qPow(x[i],2);
        }
        return x.size()*result;
    });

    QFuture<qreal> two_value = QtConcurrent::run([x]()
    {
        qreal result = 0;
        for (int i = 0; i < x.size(); i++)
        {
           result+=x[i];
        }
        return qPow(result,2);
    });

    one_value.waitForFinished();
    two_value.waitForFinished();
    return one_value.result()-two_value.result();
}

qreal find_a(QVector<qreal> &x, QVector<qreal> &y)
{
    return find_nominator_function_a(x,y)/find_denominator_function(x);
}

qreal find_b(QVector<qreal> &x, QVector<qreal> &y)
{
    return find_nominator_function_b(x,y)/find_denominator_function(x);
}

qreal get_point_y(qreal x, qreal a, qreal b)
{
    return a*x+b;
}
