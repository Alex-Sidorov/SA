#ifndef FUNCTION_H
#define FUNCTION_H

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QByteArray>
#include <QPair>
#include <QVector>
#include <tuple>
#include <math.h>
#include <QtDebug>

constexpr auto PRECISION_NEW_CENTRE = 0.5;
constexpr auto COUNT_TEXT_THEME = 12;
constexpr auto COUNT_THEME = 3;
constexpr auto COUNT_LEARN_TEXT = 7;
constexpr auto SIZE_BUG_WORDS = 60;

struct Coordinates
{
    qreal x;
    qreal y;
    qreal z;
};

qreal get_distance(const Coordinates &first_point,const  Coordinates &second_point);
bool is_false_value(QByteArray &value);
void delete_false_words(QStringList &new_words);
void delete_false_symbol(QStringList &new_words);
bool get_words_with_count(QFile &file, QVector<QPair<QString,int>> &all_words);
void insert_new_word(QVector<QPair<QString,int>> &words, QString &word);
void insert_new_word(QVector<QPair<QString,int>> &words, QPair<QString,int> &word);
void insert_new_words(QVector<QPair<QString,int>> &words, QStringList &new_words);
void insert_new_words(QVector<QPair<QString,int>> &words, QVector<QPair<QString,int>> &new_words);
int count_true_word_text(QFile &file, QVector<QPair<QString,int>> &words);
bool count_true_words_in_text(int theme,
                              int count_text,
                              QVector<QPair<QString,int>> &words,
                              QVector<int> &points);
bool create_bug_words(int theme, int count_text, int, QVector<QPair<QString,int>> &bug_words);
QVector<Coordinates> get_coordinates(QVector<int> &x, QVector<int> &y, QVector<int> &z);

std::tuple<QVector<Coordinates>,QVector<Coordinates>,QVector<Coordinates>>
create_clasters(const QVector<Coordinates> &data, QVector<Coordinates> &points);
void clasterization(const QVector<Coordinates> &data, QVector<Coordinates> &centre);
QVector<QPair<Coordinates,int>> create_result_clastarization(const QVector<Coordinates> &data, QVector<Coordinates> &centre);
QVector<QPair<Coordinates,bool>> result_clastarization();


//////////////////

/*template <class Type>
void insert_new_word1(QVector<Type> &words,Type &word)
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
}*/

template <class Type>
Type merge_bags(Type &last)
{
    return last;
}

template <class Type, class... Other>
Type merge_bags(Type &first, Other& ... other)
{
    auto result = merge_bags(other...);
    auto temp = first;
    foreach(auto item, result)
    {
        insert_new_word(temp,item);
    }
    return temp;
}

///////////////////

const QVector<char> CORRECT_SYMBOL =
{
    'A','B','C','D','E','F','G','H','Q','W','R','T','Y','U','I','O','P','S','J',
    'K','L','Z','X','V','N','M',
    'a','b','c','d','e','f','g','h','q','w', 'r','t','y','u','i','o','p','s','j',
    'k','l','z','x','v','n','m'
};

const QVector<char> FALSE_SYMBOL =
{
    '.',
    ',',
    '!',
    '?',
    '*',
    '/',
    '`',
    '+',
    '-',
    '.',
    ',',
    '{',
    '}',
    '(',
    ')',
    '[',
    ']',
    '\n',
    '\r',
    '\t'
};

const QVector<QString> FALSE_WORDS =
{
    "a",
    "an",
    "the",
    "and",
    "or",
    "of",
    "near",
    "is",
    "are",
    "was",
    "were",
    "it",
    "he",
    "she",
    "you",
    "her",
    "his",
    "your",
    "thei",
    "their",
    "in",
    "as",
    "to",
    "so",
    "from",
    "that",
    "on",
    "which",
    "be",
    "after",
    "before",
    "have",
    "how",
    "one",
    "do",
    "what",
    "might",
    "into","at","change","often","has","been","use","than","when","over","much",
    "find","first","long","by","faith","there","about","through"
    "two","etc","essence","for","this","not","we","can","same","retire","only","they","cpus",
    "all","but","ie","if","both","eg","between","also","with","these","more"
};

const QVector<char> NUMBERS =
{
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
};
#endif // FUNCTION_H
