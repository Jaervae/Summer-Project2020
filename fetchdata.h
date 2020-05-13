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
    void removeById(int id);
    bool getSearchStatus();
    void getNewEntryID(QString name);
    int getWantedID(){return this->wantedID;};

    QList<Contact> getList();

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QList<Contact> contactlist;
    QString wantedItem;

    bool dataSearchDone;
    int wantedID;

private slots:
    void managerFinished(QNetworkReply *reply);
    void replyFinished(QNetworkReply *reply);
    void getnewest(QNetworkReply *reply);
    void deleteSlot(QNetworkReply *reply);

};

#endif // FETCHDATA_H
