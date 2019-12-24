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

qreal dispersion_value(QVector<qreal> &values)
{
    qreal result = 0;
    auto expected = expected_value(values);
    foreach(auto item,values)
    {
        result+=pow(item-expected,2);
        //result +=(item-expected)*(item-expected);
    }
    return result/(values.count()-1);
}

qreal criterion_t_value(QVector<qreal> &values_one,QVector<qreal> &values_two)
{
    auto expected_one = QtConcurrent::run(expected_value,values_one);
    auto expected_two = QtConcurrent::run(expected_value,values_two);
    auto dispersion_one = QtConcurrent::run(dispersion_value,values_one);
    auto dispersion_two = dispersion_value(values_two);
    expected_one.waitForFinished();
    expected_two.waitForFinished();
    dispersion_one.waitForFinished();
    auto result = (expected_one.result()-expected_two.result())/
            sqrt((dispersion_one.result()/values_one.count())+(dispersion_two/values_two.count()));
    return result < 0 ? result * -1 : result;
}

qreal criterion_z_value(QVector<qreal> &values_one,QVector<qreal> &values_two)
{
    auto expected_one = QtConcurrent::run(expected_value,values_one);
    auto expected_two = QtConcurrent::run(expected_value,values_two);
    auto dispersion_one = QtConcurrent::run(dispersion_value,values_one);
    auto dispersion_two = dispersion_value(values_two);
    expected_one.waitForFinished();
    expected_two.waitForFinished();
    dispersion_one.waitForFinished();
    auto result = (expected_one.result()-expected_two.result())/
            (dispersion_one.result()+dispersion_two)*
            sqrt(values_one.count()*values_two.count()/(values_one.count()+values_two.count()));
    return result < 0 ? result * -1 : result;
}
