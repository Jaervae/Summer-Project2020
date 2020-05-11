#include "fetchdata.h"
#include <QNetworkReply>
#include <QtNetwork>
#include <QNetworkAccessManager>

FetchData::FetchData()
{
    this->dataSearchDone = false;
    manager = new QNetworkAccessManager();
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(managerFinished(QNetworkReply*)));
}

FetchData::~FetchData()
{
    delete manager;
}

void FetchData::getData(){
    qDebug()<< "connecting";
    request.setUrl(QUrl("https://qtphone.herokuapp.com/contact"));
    manager->get(request);
}

QList<Contact> FetchData::getList()
{
    return this->contactlist;
}

bool FetchData::getSearchStatus()
{
    return this->dataSearchDone;
}


void FetchData::managerFinished(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QString answer = reply->readAll();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(answer.toUtf8());
    QJsonArray jsonArray = jsonResponse.array();
    contactlist.clear();
    foreach (const QJsonValue & v, jsonArray)
    {
        QJsonObject obj = v.toObject();
        QJsonValue id = obj.value("id");
        if (!id.isUndefined())
            qDebug() << id.toInt();
        qDebug() << obj.value("firstname").toString() + " " + obj.value("lastname").toString();
        qDebug() << "Mobile:" + obj.value("mobile").toString() + ", Email:" + obj.value("email").toString();
        Contact contact = Contact(obj.value("id").toInt(),
                                      obj.value("firstname").toString(),
                                      obj.value("lastname").toString(),
                                      obj.value("mobile").toString(),
                                      obj.value("email").toString());
        contactlist.append(contact);

    }
    this->dataSearchDone = true;

}

