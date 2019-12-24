#include "function.h"

bool get_words_with_count(QFile &fil, QVector<QPair<QString,int>> &all_words)
{
    if(!fil.isOpen())
    {
        return false;
    }

    QTextStream file(&fil);

    //char end = 0;
    do
    {
        auto words = file.readLine().split(' ');
        if(words.isEmpty())
        {
            break;
        }
        //end = words.back().back();
        std::transform(words.begin(),words.end(),words.begin(),[](QString &value)
        {
            return value.toLower();
        });
        delete_false_symbol(words);
        delete_false_words(words);
        if(!words.isEmpty())
        {
            insert_new_words(all_words, words);
        }
    }while(!file.atEnd() && file.status()!=QTextStream::Status::ReadPastEnd);

    /*std::sort(all_words.begin(),all_words.end(),
          [](QPair<QString,int> &first , QPair<QString,int> &second)
    {
        return first.second > second.second;
    });*/

    return true;
}

bool is_false_value(QByteArray &value)
{
    auto size = FALSE_WORDS.size();
    for (auto i = 0; i < size; i++)
    {
        if(value == FALSE_WORDS[i])
        {
            return true;
        }
    }
    size = NUMBERS.size();
    for (auto i = 0; i < size; i++)
    {
        if(value[0] == NUMBERS[i])
        {
            return true;
        }
    }
    size = FALSE_SYMBOL.size();
    for (auto i = 0; i < size; i++)
    {
        if(value[0] == FALSE_SYMBOL[i])
        {
            return true;
        }
    }
    return false;
}

void delete_false_words(QStringList &new_words)
{
    for(auto i = 0; i < new_words.size();)
    {
        if(new_words[i].isEmpty() || FALSE_WORDS.count(new_words[i])/*is_false_value(new_words[i])*/)
        {
            new_words.removeAt(i);
            continue;
        }
        i++;
    }
}

void delete_false_symbol(QStringList &new_words)
{
    auto size = new_words.size();
    int index = 0;
    for(auto i = 0; i < size; i++)
    {
        /*while(index<FALSE_SYMBOL.count() && !new_words[i].isEmpty())
        {
            while (new_words[i].count(FALSE_SYMBOL[index]))
            {
                new_words[i].remove(new_words[i].indexOf(FALSE_SYMBOL[index]),1);
            }
            index++;
        }
        index = 0;*/

        while(!new_words[i].isEmpty() && index < new_words[i].size())
        {
            if(!CORRECT_SYMBOL.count(new_words[i][index].toLatin1()))
            {
                new_words[i].remove(index,1);
            }
            else
            {
                index++;
            }
        }
        index = 0;

        /*while(!new_words[i].count(NUMBERS[index]) && !new_words[i].isEmpty())
        {
            while (!new_words[i].count(NUMBERS[index]))
            {
                new_words[i].remove(new_words[i].indexOf(NUMBERS[index]),1);
            }
            index++;
        }
        index = 0;*/
    }
}

void insert_new_word(QVector<QPair<QString,int>> &words, QString &word)
{
    auto size = words.size();
    for (auto i = 0; i < size; i++)
    {
        if(words[i].first == word)
        {
            words[i].second++;
            return;
        }
    }
    words.push_back(QPair<QString,int>(word,1));
}

void insert_new_words(QVector<QPair<QString,int>> &words, QStringList &new_words)
{
    foreach(auto item, new_words)
    {
        insert_new_word(words,item);
    }
}

void insert_new_word(QVector<QPair<QString,int>> &words, QPair<QString,int> &word)
{
    auto size = words.size();
    for (auto i = 0; i < size; i++)
    {
        if(words[i].first == word.first)
        {
            words[i].second+=word.second;
            return;
        }
    }
    words.push_back(word);
}


void insert_new_words(QVector<QPair<QString,int>> &words, QVector<QPair<QString,int>> &new_words)
{
    foreach(auto item, new_words)
    {
        insert_new_word(words,item);
    }
}

int count_true_word_text(QFile &file, QVector<QPair<QString,int>> &words)
{
    if(!file.isOpen())
    {
        return 0;
    }

    QVector<QPair<QString,int>> all_word;
    get_words_with_count(file,all_word);

    auto count_all_words = all_word.count();
    auto count_true_words = words.count();

    int result = 0;
    for (auto i = 0; i < count_all_words; i++)
    {
        for (auto j = 0; j < count_true_words; j++)
        {
            if(words[j].first == all_word[i].first)
            {
                result += all_word[i].second;
                break;
            }
        }
    }
    return result;
}

qreal get_distance(const Coordinates &first_point, const Coordinates &second_point)
{
    return sqrt(pow((first_point.x - second_point.x),2) +
                pow((first_point.y - second_point.y),2) +
                pow((first_point.z - second_point.z),2));
}

Coordinates find_new_centr(const QVector<Coordinates> &claster, Coordinates &centre)
{
    auto count = claster.count();
    /*qreal max_distance = 0;
    qreal temp_distance = 0;
    Coordinates first = {0,0,0};
    Coordinates second = {0,0,0};
    for (auto i = 0; i < count - 1; i++)
    {
        for (auto j = i + 1; j < count; j++)
        {
            temp_distance = get_distance(claster[i],claster[j]);
            if(max_distance < temp_distance)
            {
                max_distance = temp_distance;
                first = claster[i];
                second = claster[j];
            }
        }
    }
    Coordinates new_centre = {0,0,0};

    new_centre.x = first.x < second.x ? first.x + abs(first.x - second.x)/2
                                      : first.x - abs(first.x - second.x)/2;
    new_centre.y = first.y < second.y ? first.y + abs(first.y - second.y)/2
                                      : first.y - abs(first.y - second.y)/2;
    new_centre.z = first.z < second.z ? first.z + abs(first.z - second.z)/2
                                      : first.z - abs(first.z - second.z)/2;*/
    Coordinates new_centre = {0,0,0};
    for (auto i = 0; i < count; i++)
    {
        new_centre.x += claster[i].x;
        new_centre.y += claster[i].y;
        new_centre.z += claster[i].z;
    }
    new_centre.x/=count;
    new_centre.y/=count;
    new_centre.z/=count;
    return new_centre;
}

std::tuple<QVector<Coordinates>,QVector<Coordinates>,QVector<Coordinates>>
create_clasters(const QVector<Coordinates> &data, QVector<Coordinates> &points)
{
    auto count_data = data.count();
    auto count_clasters = points.count();
    std::tuple<QVector<Coordinates>,QVector<Coordinates>,QVector<Coordinates>> clasters;
    auto number_claster = 0;
    auto temp = 0.0;
    auto min_distance = -1.0;
    for (auto i = 0; i < count_data; i++)
    {
        for(auto j = 0; j < count_clasters; j++)
        {
            temp = get_distance(data[i],points[j]);
            if(temp < min_distance || min_distance == -1)
            {
                number_claster = j;
                min_distance = temp;
            }
        }
        switch (number_claster)
        {
        case 0:std::get<0>(clasters).push_back(data[i]);break;
        case 1:std::get<1>(clasters).push_back(data[i]);break;
        case 2:std::get<2>(clasters).push_back(data[i]);break;
        }
        min_distance = -1.0;
    }
    return clasters;
}

bool is_end_centre(QVector<Coordinates> &old_centre, QVector<Coordinates> &new_centre)
{
    auto count = old_centre.count();
    for (auto i = 0; i <count; i++)
    {
        if(get_distance(old_centre[i],new_centre[i]) > PRECISION_NEW_CENTRE)
        {
            return false;
        }
        /*if(abs(get_distance(temp,old_centre[i])-get_distance(temp,new_centre[i]))>PRECISION_NEW_CENTRE)
        {
            return false;
        }*/
    }
    return true;
}

/*std::tuple<QVector<Coordinates>,QVector<Coordinates>,QVector<Coordinates>>
clasterization(const QVector<Coordinates> &data, QVector<Coordinates> &centre)
{
    QVector<Coordinates> old_centre;
    QVector<Coordinates> new_centre = centre;
    std::tuple<QVector<Coordinates>,QVector<Coordinates>,QVector<Coordinates>> clasters;
    do
    {
        old_centre = new_centre;
        clasters = create_clasters(data,old_centre);
        new_centre[0] = find_new_centr(std::get<0>(clasters));
        new_centre[1] = find_new_centr(std::get<1>(clasters));
        new_centre[2] = find_new_centr(std::get<2>(clasters));
    }while (!is_end_centre(old_centre,new_centre));
    centre = new_centre;
    return clasters;
}*/

void clasterization(const QVector<Coordinates> &data, QVector<Coordinates> &centre)
{
    QVector<Coordinates> old_centre;
    std::tuple<QVector<Coordinates>,QVector<Coordinates>,QVector<Coordinates>> clasters;
    do
    {
        old_centre = centre;
        clasters = create_clasters(data,old_centre);
        centre[0] = find_new_centr(std::get<0>(clasters),old_centre[0]);
        centre[1] = find_new_centr(std::get<1>(clasters),old_centre[1]);
        centre[2] = find_new_centr(std::get<2>(clasters),old_centre[2]);
    }while (!is_end_centre(old_centre,centre));
}

bool create_bug_words(int theme, int count_text, int count_words, QVector<QPair<QString,int>> &bug_words)
{
    QString path = "D:\\projectQt\\SA_4\\file\\";
    QFile file;
    QVector<QPair<QString,int>> all_words;
    for(int i = 1; i <= count_text; i++)
    {
        file.setFileName(path + QString::number(theme)+QString::number(i)+".txt");
        if(!file.open(QIODevice::ReadOnly))
        {
            return false;
        }
        get_words_with_count(file,all_words);
        bug_words = merge_bags(all_words,bug_words);
        file.close();
        all_words.clear();
    }
    std::sort(bug_words.begin(),bug_words.end(),
              [](QPair<QString,int> &first , QPair<QString,int> &second)
        {
            return first.second > second.second;
        });
    if(bug_words.count() > count_words)
    {
        bug_words.resize(count_words);
    }
    return true;
}

bool count_true_words_in_text(int theme,
                              int count_text,
                              QVector<QPair<QString,int>> &words,
                              QVector<int> &points)
{
    QString path = "D:\\projectQt\\SA_4\\file\\";
    QFile file;
    for(int i = 1; i <= count_text; i++)
    {
        file.setFileName(path + QString::number(theme)+QString::number(i)+".txt");
        if(!file.open(QIODevice::ReadOnly))
        {
            points.clear();
            return false;
        }
        points[i - 1] = count_true_word_text(file,words);
        file.close();
    }
    return true;
}


QVector<Coordinates> get_coordinates(QVector<int> &x, QVector<int> &y, QVector<int> &z)
{
    auto count_points = x.count();
    QVector<Coordinates> points(count_points);
    for(int i = 0; i < count_points; i++)
    {
        points[i].x = x[i];
        points[i].y = y[i];
        points[i].z = z[i];
    }
    return points;
}

QVector<QPair<Coordinates,int>> create_result_clastarization(const QVector<Coordinates> &data, QVector<Coordinates> &centre)
{
    QVector<QPair<Coordinates,int>> result(data.count());
    int index = 0;
    for(auto i = 0; i < COUNT_THEME; i++)
    {
        auto temp = data.mid(i*COUNT_TEXT_THEME,COUNT_TEXT_THEME);
        auto clasters = create_clasters(temp,centre);
        auto claster = std::get<0>(clasters);

        for (auto j = 0; j<claster.count();j++)
        {
            result[index] = qMakePair(claster[j],1);
            /*if(0==i)
            {
                result[index] = qMakePair(claster[j],true);
            }
            else
            {
                result[index] = qMakePair(claster[j],false);
            }
            index++;*/
            index++;
        }
        claster = std::get<1>(clasters);
        for (auto j = 0; j<claster.count();j++)
        {
            /*if(1==i)
            {
                result[index] = qMakePair(claster[j],true);
            }
            else
            {
                result[index] = qMakePair(claster[j],false);
            }
            index++;*/
            result[index] = qMakePair(claster[j],2);
            index++;
        }
        claster = std::get<2>(clasters);
        for (auto j = 0; j<claster.count();j++)
        {
            /*if(2==i)
            {
                result[index] = qMakePair(claster[j],true);
            }
            else
            {
                result[index] = qMakePair(claster[j],false);
            }
            index++;*/
            result[index] = qMakePair(claster[j],3);
            index++;
        }
    }
    return result;
}

/*QVector<QPair<Coordinates,bool>> result_clastarization()
{
    QVector<QPair<QString,int>> first;
    QVector<QPair<QString,int>> second;
    QVector<QPair<QString,int>> third;
    if(!create_bug_words(1,4,60,first)
            || !create_bug_words(2,4,60,second)
            || !create_bug_words(3,4,60,third))
    {
        return {};
    }

    foreach(auto i, first)
    {
        qDebug()<<i;
    }
    foreach(auto i, second)
    {
        qDebug()<<i;
    }
    foreach(auto i, third)
    {
        qDebug()<<i;
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
        return{};
    }

    auto first_data = get_coordinates(first_points_x,first_points_y,first_points_z);
    auto second_data = get_coordinates(second_points_x,second_points_y,second_points_z);
    auto third_data = get_coordinates(third_points_x,third_points_y,third_points_z);

    auto temp_data = first_data.mid(0,4) + second_data.mid(0,4) + third_data.mid(0,4);
    auto centre = get_rand_centre(300,300,300);
    centre[0] = temp_data[2];
    centre[1] = temp_data[6];
    centre[2] = temp_data[10];
    clasterization(temp_data,centre);

    temp_data = first_data + second_data + third_data;
    return create_result_clastarization(temp_data,centre);
}*/






























