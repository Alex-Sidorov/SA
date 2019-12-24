#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QUrl>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSsl>
#include <QString>
#include <QVector>

struct DataFile
{
    QByteArray col_one;
    QByteArray col_two;
    QByteArray col_three;
    QByteArray col_four;
    QByteArray col_five;
};

class Parser: public QObject
{
    Q_OBJECT

public:
    Parser();
    explicit Parser(QUrl &url, QObject *parent = nullptr);
    virtual ~Parser();

    void set_url(QUrl &url);
    QUrl get_url()const;
    QByteArray get_data()const;
    const QVector<DataFile>& get_parsed_data()const;

public slots:
    void start();
    void slot_dowland_data(QNetworkReply*);
    void parsing();

signals:
    void downloaded();

private:
    QNetworkAccessManager _manager;
    QUrl _url;
    QByteArray _data;
    QVector<DataFile> _parsed_data;
    QNetworkReply *_reply;
};



#endif // PARSER_H
