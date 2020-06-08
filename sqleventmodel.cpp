#include "sqleventmodel.h"

#include <QDebug>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QNetworkReply>
#include <QtNetwork>
#include <QNetworkAccessManager>

SqlEventModel::SqlEventModel()
{
    createConnection();
}

QList<QObject*> SqlEventModel::eventsForDate(const QDate &date)
{
    const QString queryStr = QString::fromLatin1("SELECT * FROM Event WHERE '%1' >= startDate AND '%1' <= endDate").arg(date.toString("yyyy-MM-dd"));
    QSqlQuery query(queryStr);
    if (!query.exec())
        qFatal("Query failed");

    QList<QObject*> events;
    while (query.next()) {
        Event *event = new Event(this);
        event->setName(query.value("name").toString());

        QDateTime startDate;
        startDate.setDate(query.value("startDate").toDate());
        startDate.setTime(QTime(0, 0).addSecs(query.value("startTime").toInt()));
        event->setStartDate(startDate);

        QDateTime endDate;
        endDate.setDate(query.value("endDate").toDate());
        endDate.setTime(QTime(0, 0).addSecs(query.value("endTime").toInt()));
        event->setEndDate(endDate);

        events.append(event);
    }

    return events;
}

/*
    Defines a helper function to open a connection to an
    in-memory SQLITE database and to create a test table.
*/
void SqlEventModel::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        qFatal("Cannot open database");
        return;
    }


    QSqlQuery query;

    // We store the time as seconds because it's easier to query.
    query.exec("create table Event (name TEXT, startDate DATE, startTime INT, endDate DATE, endTime INT)");
    /*query.prepare("create table Event (name, startDate, startTime, endDate, endTime)"
                                "VALUES (:name, :startDate, :startTime, :endDate, :endTime)");

    for (int i = 0; i < eventList.length(); i++){
        query.bindValue(":name", eventList[i]->name());
        query.bindValue(":startDate", eventList[i]->startDate().Da);
        query.bindValue(":startTime", eventList[i]->name());
        query.bindValue(":endDate", eventList[i]->name());
        query.bindValue(":endTime", eventList[i]->name());
        query.exec();
    }*/

    query.exec("insert into Event values('Grocery shopping', '2014-01-01', 36000, '2014-01-01', 39600)");
    query.exec("insert into Event values('Ice skating', '2014-01-01', 57600, '2014-01-01', 61200)");
    query.exec("insert into Event values('Doctor''s appointment', '2014-01-15', 57600, '2014-01-15', 63000)");
    query.exec("insert into Event values('Conference', '2014-01-24', 32400, '2014-01-28', 61200)");

    return;
}

bool SqlEventModel::saveChanges()
{
    QJsonArray array;
    for (int i = 0; i < eventList.length(); i++){
        QString startDate = eventList[i]->startDate().date().toString();
        int startTime = eventList[i]->startDate().time().second();
        QString endDate = eventList[i]->endDate().date().toString();
        int endTime = eventList[i]->endDate().time().second();
        QJsonObject eventObj;
        eventObj["name"] = eventList[i]->name();
        eventObj["start-date"] = startDate;
        eventObj["start-time"] = startTime;
        eventObj["end-date"] = endDate;
        eventObj["end-time"] = endTime;
        array.append(eventObj);
    }
    QJsonObject object;
    object["Events"] = array;
    QJsonDocument saveDoc(object);
    qDebug()<<array;

    QFile saveFile(QStringLiteral("events.json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    saveFile.write(QJsonDocument(array).toJson());

    qDebug()<<"Done saving";

    return true;
}

bool SqlEventModel::loadEvents()
{
    /*
    QFile file(QStringLiteral("events.json"));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = jsonResponse.array();

    qDebug()<< jsonArray;
    */
    return true;
}
