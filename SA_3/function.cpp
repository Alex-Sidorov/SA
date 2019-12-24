#include "function.h"

QVector<QRgb> to_grayscale(QImage &image)
{
    auto width = image.width();
    auto height = image.height();
    QVector<QRgb> vector_rgb(width*height);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            auto value = qGray(image.pixel(j,i));
            vector_rgb[i*height+j] = qRgb(value,value,value);
        }
    }
    return vector_rgb;
}

qreal expected_value(QVector<QRgb> &data)
{
    qreal result = 0;
    foreach(auto item, data)
    {
        result += qGray(item);
    }
    return result/data.size();
}

qreal expected_value(QVector<int> &data)
{
    qreal result = 0;
    foreach(auto item, data)
    {
        result += item;
    }
    return result/data.size();
}

QPair<int,int> mode_value(QMap<QPair<int,int>,int> &map)
{
    return map.key(std::max_element(map.begin(),map.end()).value());
}

int mode_value(QMap<QPair<int,int>,int> &map,QVector<QRgb> &data)
{
    auto interval = map.key(std::max_element(map.begin(),map.end()).value());
    auto max = 0;
    auto result = 0;
    for (auto i = interval.first; i < interval.second;i++)
    {
        auto temp = data.count(qRgb(i,i,i));
        if(max < temp)
        {
            max = temp;
            result = i;
        }
    }
    return result;
}

/*qreal median_value(QVector<QRgb> &data)
{
    auto temp = data;
    std::sort(temp.begin(),temp.end());
    qDebug()<<temp.size();
    qDebug()<<temp.size()/2;
    qDebug()<<qGray(temp[temp.size()/2]);
    return qGray(temp[temp.size()/2]);
}*/
qreal median_value(QMap<QPair<int,int>,int> &histogram)
{
    auto sum_values = 0;
    foreach(auto item,histogram)
    {
        sum_values += item;
    }
    auto sum_value = 0;
    QPair<int,int> key;
    foreach(auto item,histogram)
    {
        sum_value+=item;
        if(sum_value>=sum_values/2)
        {
            key = histogram.key(item);
            break;
        }
    }

    return key.first+12;
}


qreal standart_deviation_value(QVector<QRgb> &data)
{
    auto expected = expected_value(data);
    qreal result = 0;
    foreach(auto item,data)
    {
        result += pow(qGray(item)-expected,2);
    }
    return sqrt(result/data.size());
}

qreal correlation_value(QImage &first, QImage &second)
{
    auto first_gray = to_grayscale(first);
    auto second_gray = to_grayscale(second);
    QFuture<qreal> nominator = QtConcurrent::run(find_nominator_correlation_image,first_gray,second_gray);
    QFuture<qreal> denominator = QtConcurrent::run(find_denominator_correlation_image,first_gray,second_gray);
    return nominator.result()/denominator.result();

}

QMap<QPair<int,int>,int> create_interval_histogram(QVector<QRgb> &grayscale_image)
{
    QMap<QPair<int,int>,int> map;
    QMap<qreal,int> temp_map;
    /*std::transform(grayscale_image.begin(),grayscale_image.end(),std::back_inserter(map),
                   [&grayscale_image](QRgb &value){return QPair<qreal,int>(qGray(value),grayscale_image.count(value));});*/
    QSet<QRgb> set;
    set.reserve(256);
    foreach(auto item,grayscale_image)
    {
        set.insert(item);
    }
    /*foreach(auto item,set)
    {
        temp_map.insert(item,grayscale_image.count(item));
    }
    for (int i = 0; i < 256; i+=8)
    {
        map.insert(qMakePair(i,i+8),get_count_values_interval(temp_map,qMakePair(i,i+8)));
    }*/
    for (int i = 0; i < 256; i+=8)
    {
        map.insert(qMakePair(i,i+8),get_count_values_interval(grayscale_image,set,qMakePair(i,i+8)));
    }
    return map;
}

qreal correlation_value(QVector<QRgb> &first, QVector<QRgb> &second)
{
    auto first_histogram = create_interval_histogram(first);
    auto second_histogram = create_interval_histogram(second);
    QVector<int> first_values(256/8);
    QVector<int> second_values(256/8);
    auto iterator = second_values.begin();
    std::transform(first_histogram.begin(),first_histogram.end(),second_histogram.begin(),first_values.begin(),
                    [&iterator](int &first_value, int &second_value)
                    {
                        (*iterator) = second_value;
                        iterator++;
                        return first_value;
                    });
    QFuture<qreal> nominator = QtConcurrent::run(find_nominator_correlation_histogram,first_values,second_values);
    QFuture<qreal> denominator = QtConcurrent::run(find_denominator_correlation_histogram,first_values,second_values);
    return nominator.result()/denominator.result();
}

int get_count_values_interval(QVector<QRgb> &grayscale, QSet<QRgb> &intensiv,QPair<int,int> &&range)
{
    int result = 0;
    for (int i = range.first; i < range.second; i++)
    {
        auto value = qRgb(i,i,i);
        if(intensiv.contains(value))
        {
            result+=grayscale.count(value);
        }
    }
    return result;
}

qreal find_nominator_correlation_histogram(QVector<int> &x, QVector<int> &y)
{
    auto expected_x = expected_value(x);
    auto expected_y = expected_value(y);
    qreal result = 0;
    auto range = x.size()<y.size()?x.size():y.size();
    for (int i = 0; i < range; i++)
    {
        result += (x[i]-expected_x)*(y[i]-expected_y);
    }
    return result;
}

qreal find_nominator_correlation_image(QVector<QRgb> &x, QVector<QRgb> &y)
{
    auto expected_x = expected_value(x);
    auto expected_y = expected_value(y);
    qreal result = 0;
    auto range = x.size()<y.size()?x.size():y.size();
    for (int i = 0; i < range; i++)
    {
        result += (qGray(x[i])-expected_x)*(qGray(y[i])-expected_y);
    }
    return result;
}

qreal find_denominator_correlation_histogram(QVector<int> &x, QVector<int> &y)
{
    auto expected_x = expected_value(x);
    auto expected_y = expected_value(y);
    auto range = x.size()<y.size()?x.size():y.size();
    qreal x_ = 0;
    qreal y_ = 0;
    for (int i = 0; i < range; i++)
    {
        x_+= qPow(((x[i]-expected_x)),2);
        y_+= qPow(((y[i]-expected_y)),2);
    }
    return qSqrt(x_*y_);
}

qreal find_denominator_correlation_image(QVector<QRgb> &x, QVector<QRgb> &y)
{
    auto expected_x = expected_value(x);
    auto expected_y = expected_value(y);
    qreal x_ = 0;
    qreal y_ = 0;
    auto range = x.size()<y.size()?x.size():y.size();
    for (int i = 0; i < range; i++)
    {
        x_+= qPow(((qGray(x[i])-expected_x)),2);
        y_+= qPow(((qGray(y[i])-expected_y)),2);
    }
    return qSqrt(x_*y_);
}

qreal expected_value(QVector<QPair<int,qreal>> &data)
{
    qreal result = 0;
    foreach(auto item,data)
    {
        result += item.first*item.second;
    }
    return result;
}

qreal dispersion_value(QVector<QPair<int,qreal>> &data)
{
    auto expected = expected_value(data);
    qreal result = 0;
    foreach(auto item ,data)
    {
        result += qPow((item.first - expected),2)*item.second;
    }
    return result;
}

qreal norm_fun(qreal value,VALUES flag)
{
    switch (flag)
    {
    case PLUS_INFINITY:return 1;
    case MINUS_INFINITY:return -1;
    //default: return 0.5*erfc(-1*value/sqrt(2));
    default:
    {
        if(value<0)
        {
            return 0.5 - 0.5*erf(-1*value/sqrt(2));
        }
        else
        {
            return 0.5 + 0.5*erf(value/sqrt(2));
        }
    }
    }
}

qreal teoretic_m_value(int count, qreal sdv,qreal expected,QPair<int,int> &range)
{
    //if(range.first==0) qDebug()<<(norm_fun((range.second-expected)/sdv,VALUE));

    switch (range.first)
    {
    case 0:
    {
        auto temp = (range.second-expected) / sdv;
        auto sign = temp < 0 ? -1:1;
        //qDebug()<<(1 + sign*norm_fun(sign*temp,VALUE));
        return count*(1 + sign*norm_fun(sign*temp,VALUE));
    }
    case 248:
    {
        auto temp = (range.first-expected) / sdv;
        auto sign = temp < 0 ? -1:1;
        //qDebug()<<(1 - sign*norm_fun(sign*temp,VALUE));
        return count*(1 - sign*norm_fun(sign*temp,VALUE));
    }
    default:
    {
        auto first_temp = (range.first-expected) / sdv;
        auto second_temp = (range.second-expected) / sdv;
        auto first_sign = first_temp < 0 ? -1:1;
        auto second_sign = second_temp < 0 ? -1:1;
        /*qDebug()<<norm_fun(first_sign*first_temp,VALUE);
        qDebug()<<norm_fun(second_sign*second_temp,VALUE);
        qDebug()<<norm_fun(second_sign*second_temp,VALUE)-norm_fun(first_sign*first_temp,VALUE);*/
        return count *(norm_fun(second_sign*second_temp,VALUE)-norm_fun(first_sign*first_temp,VALUE));
    }
    }
    /*switch (range.first)
    {
    case 0:  return count * (norm_fun((range.second-expected) / sdv, VALUE) - norm_fun(1, MINUS_INFINITY));
    case 248:return count * (norm_fun(1, PLUS_INFINITY) - norm_fun((range.first-expected) / sdv, VALUE));
    default: return count * (norm_fun((range.second-expected) / sdv, VALUE) - norm_fun((range.first-expected) / sdv, VALUE));
    }*/
}

bool is_Pirson(QMap<QPair<int,int>,int> &histogram)
{
    auto sum_values = 0;
    foreach(auto item,histogram)
    {
        sum_values += item;
    }
    QVector<QPair<int,qreal>> table(histogram.size());
    std::transform(histogram.begin(),histogram.end(),table.begin(),[&sum_values,&histogram](int &value)
    {
        auto interval = histogram.key(value);
        return qMakePair((interval.first + interval.second) / 2, static_cast<qreal>(value) / sum_values);
    });
    auto sdv = sqrt(dispersion_value(table));
    auto expected = expected_value(table);
    auto Xi = 0.0;
    auto ranges = histogram.keys();
    for(int i = 0; i < 32; i++)
    {
        auto teoretic_m = teoretic_m_value(sum_values, sdv, expected,ranges[i]);
        Xi += qPow(histogram.value(ranges[i]) - teoretic_m,2) / teoretic_m;
    }
    return abs(Xi) < CRITICAL_VALUE;
}

bool is_Kolmagorov(QMap<QPair<int,int>,int> &histogram)
{
    auto sum_values = 0;
    foreach(auto item,histogram)
    {
        sum_values += item;
    }
    QVector<QPair<int,qreal>> table(histogram.size());
    std::transform(histogram.begin(),histogram.end(),table.begin(),[&sum_values,&histogram](int &value)
    {
        auto interval = histogram.key(value);
        return qMakePair((interval.first + interval.second) / 2, static_cast<qreal>(value) / sum_values);
    });

    auto sdv = sqrt(dispersion_value(table));
    auto expected = expected_value(table);

    auto acc = 0;
    auto max_sub = 0.0;
    for(int i = 0; i < 256; i+=8)
    {
        auto temp = qMakePair(i,i+8);
        acc += histogram.value(temp);
        auto temp_sub = abs(acc/sum_values-(1+0.5*erf((histogram.value(temp)-expected)/(sqrt(2)*sdv))));
        max_sub = max_sub < temp_sub ? temp_sub : max_sub;
    }
    return max_sub < LAMBDA_05;
}


