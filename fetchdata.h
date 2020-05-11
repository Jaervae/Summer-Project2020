#ifndef FETCHDATA_H
#define FETCHDATA_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>
#include "contact.h"


class FetchData : public QObject
{
    Q_OBJECT

public:
    FetchData();
    ~FetchData();

    void getData();
    QList<Contact> getList();
    bool getSearchStatus();

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QList<Contact> contactlist;
    bool dataSearchDone;

private slots:
    void managerFinished(QNetworkReply *reply);

};

#endif // FETCHDATA_H
