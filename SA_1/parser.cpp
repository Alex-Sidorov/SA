#include "parser.h"


Parser::Parser(QUrl &url, QObject *parent) : QObject (parent), _url(url)
{
    connect(&_manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_dowland_data(QNetworkReply*)));
}

Parser::Parser()
{
    connect(&_manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_dowland_data(QNetworkReply*)));
}

Parser::~Parser()
{}

void Parser::set_url(QUrl &url)
{
    _url = url;
}

QUrl Parser::get_url()const
{
    return _url;
}
QByteArray Parser::get_data()const
{
    return _data;
}

const QVector<DataFile>& Parser::get_parsed_data()const
{
    return _parsed_data;
}

void Parser::parsing()
{
    auto list = _data.split('\n');
    QList<QByteArray> temp;
    foreach(auto item, list)
    {
        if(item.isEmpty())
        {
            return;
        }
        temp = item.split(',');
        _parsed_data.push_back(DataFile{temp[0],
                                        temp[1],
                                        temp[2],
                                        temp[3],
                                        temp[4]});
    }

}

void Parser::start()
{
    QNetworkRequest request(_url);
    _reply = _manager.get(request);
}

void Parser::slot_dowland_data(QNetworkReply* reply)
{
    if(!reply->error())
    {
        _data = reply->readAll();
        reply->deleteLater();
        emit downloaded();
    }
}
