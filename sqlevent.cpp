#include "sqlevent.h"
#include "event.h"
#include "settings.h"

#include <QDebug>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QNetworkReply>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QDateTime>

SqlEvent::SqlEvent(QObject *parent) : QObject(parent)
{
    loadEvents();
    createConnection();
}

QVector<EventItem> SqlEvent::items() const
{
    return eventList;
}

QVector<EventItem> SqlEvent::currentItems() const
{
    return selectedDateEventsList;
}

bool SqlEvent::setItemAt(int index, const EventItem &item)
{
    if (index < 0 || index >= eventList.size())
        return false;

    const EventItem &oldItem = items().at(index);
    if (item.id == oldItem.id && item.eventName == oldItem.eventName)
        return false;

    eventList[index] = item;
    return true;
}

bool SqlEvent::setItemAtSelectedList(int index, const EventItem &item)
{
    if (index < 0 || index >= selectedDateEventsList.size())
        return false;

    const EventItem &oldItem = items().at(index);
    if (item.id == oldItem.id && item.eventName == oldItem.eventName)
        return false;

    selectedDateEventsList[index] = item;
    return true;
}


QList<QObject*> SqlEvent::eventsForDate(const QDate &date)
{
    const QString queryStr = QString::fromLatin1("SELECT * FROM Event WHERE '%1' >= startDate AND '%1' <= endDate").arg(date.toString("yyyy-MM-dd"));
    QSqlQuery query(queryStr);
    if (!query.exec())
        qFatal("Query failed");
    QList<QObject*> events;
    int i = 0;
    while (query.next()) {
        Event *event = new Event(this);
        event->setName(query.value("name").toString());
        event->setDataId(query.value("id").toInt());
        QDateTime startDate;
        startDate.setDate(query.value("startDate").toDate());
        startDate.setTime(QTime(0, 0).addSecs(query.value("startTime").toInt()));
        event->setStartDate(startDate);

        QDateTime endDate;
        endDate.setDate(query.value("endDate").toDate());
        endDate.setTime(QTime(0, 0).addSecs(query.value("endTime").toInt()));
        event->setEndDate(endDate);

        events.append(event);
        qDebug()<<event->name();
        i++;
    }

    return events;
}

int SqlEvent::eventcounter(const QDate &date)
{
    const QString queryStr = QString::fromLatin1("SELECT * FROM Event WHERE '%1' >= startDate AND '%1' <= endDate").arg(date.toString("yyyy-MM-dd"));
    QSqlQuery query(queryStr);
    if (!query.exec())
        qFatal("Query failed");
    int i = 0;
    while (query.next()) {
        i++;
    }

    return i;
}


void SqlEvent::newEvent(QString eventName, QString startDate, QString startTime, QString endDate, QString endTime)
{
    emit preItemAppended(eventList.length());
    EventItem event;

    event.eventName = eventName;

    Settings *settingsObj = new Settings();
    QVariant datatype = 1;
    int nextEventIDValue = settingsObj->loadData("next-event-id", datatype.toInt()).toInt();
    qDebug()<<nextEventIDValue;

    QStringList dateList = startDate.split("-");
    QStringList timeList = startTime.split(":");
    int seconds = (QTime(timeList[0].toInt(),timeList[1].toInt()).msecsSinceStartOfDay() / 1000);

    QDateTime mStartDate;
    mStartDate.setDate(QDate(dateList[2].toInt(),dateList[1].toInt(),dateList[0].toInt()));
    mStartDate.setTime(QTime(0, 0).addSecs(seconds));
    event.startDate = mStartDate;

    dateList = endDate.split("-");
    timeList = endTime.split(":");
    seconds = (QTime(timeList[0].toInt(),timeList[1].toInt()).msecsSinceStartOfDay() / 1000);

    QDateTime mEndDate;
    mEndDate.setDate(QDate(dateList[2].toInt(),dateList[1].toInt(),dateList[0].toInt()));
    mEndDate.setTime(QTime(0, 0).addSecs(seconds));
    event.endDate = mEndDate;
    event.id = nextEventIDValue;
    eventList.append(event);


    QSqlQuery query;
    query.prepare("INSERT INTO Event (id, name, startDate, startTime, endDate, endTime) "
                  "VALUES (:id, :name, :startDate, :startTime, :endDate, :endTime)");
    query.bindValue(":id", event.id);
    query.bindValue(":name", eventName);
    query.bindValue(":startDate", mStartDate.date());
    query.bindValue(":startTime", (mStartDate.time().msecsSinceStartOfDay() / 1000 ));
    query.bindValue(":endDate", mEndDate.date());
    query.bindValue(":endTime", (mEndDate.time().msecsSinceStartOfDay() / 1000 ));
    query.exec();

    saveChanges();
    //createConnection();
    emit postItemAppended();
}

void SqlEvent::appendItem(EventItem item)
{
    emit preItemAppended(eventList.length());

    selectedDateEventsList.append(item);

    emit postItemAppended();
}

void SqlEvent::removeOne(int id)
{
    int index = 0;
    for(int i = 0; i < eventList.length(); i++){
        if(eventList[i].id == id){
            index = i;
            break;
        }
    }

    emit preItemRemoved(index);

    QSqlQuery query;
    query.prepare("DELETE FROM Event WHERE id = '" + QString::number(id) + "'");
    query.exec();
    qDebug()<<index;
    eventList.removeAt(index);
    saveChanges();

    emit postItemRemoved();
}

void SqlEvent::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        qFatal("Cannot open database");
        return;
    }

    QSqlQuery query;
    query.exec("create table Event (id INT, name TEXT, startDate DATE, startTime INT, endDate DATE, endTime INT)");
    query.prepare("INSERT INTO Event (id, name, startDate, startTime, endDate, endTime) "
                  "VALUES (:id, :name, :startDate, :startTime, :endDate, :endTime)");
    for (int i = 0; i < eventList.length(); i++){
        query.bindValue(":id", eventList[i].id);
        query.bindValue(":name", eventList[i].eventName);
        query.bindValue(":startDate", eventList[i].startDate.date());
        query.bindValue(":startTime", (eventList[i].startDate.time().msecsSinceStartOfDay() / 1000 ));
        query.bindValue(":endDate", eventList[i].endDate.date());
        query.bindValue(":endTime", (eventList[i].endDate.time().msecsSinceStartOfDay() / 1000 ));
        query.exec();
    }
    return;
}

bool SqlEvent::saveChanges()
{
    emit preItemSave();

    QJsonArray array;
    for (int i = 0; i < eventList.length(); i++){
        QString startDate = QStringLiteral("%1-%2-%3").arg(eventList[i].startDate.date().year())
                .arg(eventList[i].startDate.date().month())
                .arg(eventList[i].startDate.date().day());
        QString endDate = QStringLiteral("%1-%2-%3").arg(eventList[i].endDate.date().year())
                .arg(eventList[i].endDate.date().month())
                .arg(eventList[i].endDate.date().day());
        QJsonObject eventObj;
        eventObj["id"] = eventList[i].id;
        eventObj["name"] = eventList[i].eventName;
        eventObj["start-date"] = startDate;
        eventObj["start-time"] = (eventList[i].startDate.time().msecsSinceStartOfDay() / 1000 );
        eventObj["end-date"] =  endDate;
        eventObj["end-time"] = (eventList[i].endDate.time().msecsSinceStartOfDay() / 1000 );
        array.append(eventObj);
    }
    QJsonObject object;
    object["Events"] = array;
    QJsonDocument saveDoc(object);
    //qDebug()<<array;

    QFile saveFile(QStringLiteral("events.json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    saveFile.write(QJsonDocument(array).toJson());

    //qDebug()<<"Done saving";

    emit postItemSave();
    return true;
}

bool SqlEvent::loadEvents()
{

    QFile file(QStringLiteral("events.json"));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = jsonResponse.array();

    if(eventList.size() > 0){
        while(eventList.size() > 0){
            removeOne(0);
        }
    }


    foreach (const QJsonValue & v, jsonArray){
        EventItem event;
        event.eventName = v.toObject().value("name").toString();
        QString str = v.toObject().value("start-date").toString();
        QStringList strList = str.split("-");
        QDateTime startDate;
        startDate.setDate(QDate(strList[0].toInt(),strList[1].toInt(),strList[2].toInt()));
        startDate.setTime(QTime(0, 0).addSecs(v.toObject().value("start-time").toInt()));
        event.startDate = startDate;

        str = v.toObject().value("end-date").toString();
        strList = str.split("-");
        QDateTime endDate;
        endDate.setDate(QDate(strList[0].toInt(),strList[1].toInt(),strList[2].toInt()));
        endDate.setTime(QTime(0, 0).addSecs(v.toObject().value("end-time").toInt()));
        event.endDate = endDate;
        event.id = v.toObject().value("id").toInt();
        eventList.append(event);
    }

    return true;
}
