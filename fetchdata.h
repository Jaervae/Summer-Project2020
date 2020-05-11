#ifndef FETCHDATA_H
#define FETCHDATA_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>

class FetchData : public QObject
{
    Q_OBJECT
public:
    FetchData();
    ~FetchData();

    void getData();

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;

private slots:
    void managerFinished(QNetworkReply *reply);

};

#endif // FETCHDATA_H
