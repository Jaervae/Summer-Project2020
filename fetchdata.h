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
    void putData(Contact contact, bool isNew);
    bool getSearchStatus();
    QList<Contact> getList();


private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QList<Contact> contactlist;
    bool dataSearchDone;

private slots:
    void managerFinished(QNetworkReply *reply);
    void replyFinished(QNetworkReply *reply);

};

#endif // FETCHDATA_H
