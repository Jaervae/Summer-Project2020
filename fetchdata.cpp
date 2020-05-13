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
    request.setUrl(QUrl("https://qtphone.herokuapp.com/contact"));
    manager->get(request);
}

void FetchData::putData(Contact contact, bool isNew)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QJsonObject json;
    json.insert("firstname", contact.getFN());
    json.insert("lastname", contact.getLN());
    json.insert("mobile", contact.getMobile());
    json.insert("email", contact.getEmail());

    QString name = contact.getFN() + " " + contact.getLN();

    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));

    if(isNew){
        QUrl url("https://qtphone.herokuapp.com/contact");
        QNetworkRequest request(url);

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        manager->post(request, QJsonDocument(json).toJson());
    }

    else{
        int id = NULL;
        QString url = QStringLiteral("https://qtphone.herokuapp.com/contact/35");
        QUrl qurl(url);
        QNetworkRequest request(qurl);

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        manager->put(request, QJsonDocument(json).toJson());
    }
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

void FetchData::replyFinished(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QString answer = reply->readAll();
    qDebug() << answer.toUtf8();
    getData();

}

