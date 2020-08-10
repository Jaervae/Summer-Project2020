#include "sqlevent.h"
#include "event.h"

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
        qDebug()<<event->name();
    }

    return events;
}


void SqlEvent::newEvent(QString eventName, QString startDate, QString startTime, QString endDate, QString endTime)
{
    emit preItemAppended(eventList.length());
    EventItem event;

    //event.setName(eventName);
    event.eventName = eventName;

    QStringList dateList = startDate.split("-");
    QStringList timeList = startTime.split(":");
    int seconds = (QTime(timeList[0].toInt(),timeList[1].toInt()).msecsSinceStartOfDay() / 1000);

    QDateTime mStartDate;
    mStartDate.setDate(QDate(dateList[2].toInt(),dateList[1].toInt(),dateList[0].toInt()));
    mStartDate.setTime(QTime(0, 0).addSecs(seconds));
    //event->setStartDate(mStartDate);
    event.startDate = mStartDate;
    //qDebug()<<mStartDate;

    dateList = endDate.split("-");
    timeList = endTime.split(":");
    seconds = (QTime(timeList[0].toInt(),timeList[1].toInt()).msecsSinceStartOfDay() / 1000);

    QDateTime mEndDate;
    mEndDate.setDate(QDate(dateList[2].toInt(),dateList[1].toInt(),dateList[0].toInt()));
    mEndDate.setTime(QTime(0, 0).addSecs(seconds));
    //event->setEndDate(mEndDate);
    event.endDate = mEndDate;
    //qDebug()<<mEndDate;
    //event->setDataId(eventList.size());
    event.id = eventList.length();
    eventList.append(event);
    //qDebug()<<"New event added";

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

void SqlEvent::removeOne(int index)
{
    emit preItemRemoved(index);

    eventList.removeAt(index);
    saveChanges();

    emit postItemRemoved();
}

SqlEvent *SqlEvent::getCurrentEventsForSelectedDate(const QDate &date)
{
    const QString queryStr = QString::fromLatin1("SELECT * FROM Event WHERE '%1' >= startDate AND '%1' <= endDate").arg(date.toString("yyyy-MM-dd"));
    QSqlQuery query(queryStr);
    if (!query.exec())
        qFatal("Query failed");
    SqlEvent *events = new SqlEvent();
    while (query.next()) {
        EventItem *event = new EventItem();
        event->eventName = query.value("name").toString();

        QDateTime startDate;
        startDate.setDate(query.value("startDate").toDate());
        startDate.setTime(QTime(0, 0).addSecs(query.value("startTime").toInt()));
        event->startDate = startDate;

        QDateTime endDate;
        endDate.setDate(query.value("endDate").toDate());
        endDate.setTime(QTime(0, 0).addSecs(query.value("endTime").toInt()));
        event->endDate = endDate;

        events->appendItem(*event);
        qDebug()<<event->eventName;
    }
    return events;
}

void SqlEvent::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        qFatal("Cannot open database");
        return;
    }

            /*
            Event *event = new Event(this);
            event->setName("Conference");

            QDateTime startDate;
            startDate.setDate(QDate(2014,1,24));
            startDate.setTime(QTime(0, 0).addSecs(32400));
            event->setStartDate(startDate);

            QDateTime endDate;
            endDate.setDate(QDate(2014,1,28));
            endDate.setTime(QTime(0, 0).addSecs(61200));
            event->setEndDate(endDate);

            eventList.append(event);
            saveChanges();
            */

    QSqlQuery query;
    // We store the time as seconds because it's easier to query.
    query.exec("create table Event (name TEXT, startDate DATE, startTime INT, endDate DATE, endTime INT)");
    query.prepare("INSERT INTO Event (name, startDate, startTime, endDate, endTime) "
                  "VALUES (:name, :startDate, :startTime, :endDate, :endTime)");
    for (int i = 0; i < eventList.length(); i++){
        //qDebug()<< i;
        //qDebug()<< eventList[i]->startDate().date();
        //qDebug()<< (eventList[i]->startDate().time().msecsSinceStartOfDay() / 1000 );
        query.bindValue(":name", eventList[i].eventName);
        query.bindValue(":startDate", eventList[i].startDate.date());
        query.bindValue(":startTime", (eventList[i].startDate.time().msecsSinceStartOfDay() / 1000 ));
        query.bindValue(":endDate", eventList[i].endDate.date());
        query.bindValue(":endTime", (eventList[i].endDate.time().msecsSinceStartOfDay() / 1000 ));
        query.exec();
    }

    /*
    query.exec("insert into Event values('Grocery shopping', '2014-01-01', 36000, '2014-01-01', 39600)");
    query.exec("insert into Event values('Ice skating', '2014-01-01', 57600, '2014-01-01', 61200)");
    query.exec("insert into Event values('Doctor''s appointment', '2014-01-15', 57600, '2014-01-15', 63000)");
    query.exec("insert into Event values('Conference', '2014-01-24', 32400, '2014-01-28', 61200)");
    */


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

    //qDebug()<< jsonArray;


    if(eventList.size() > 0){
        while(eventList.size() > 0){
            removeOne(0);
        }
    }


    foreach (const QJsonValue & v, jsonArray){
        EventItem event;
        //event->setName(v.toObject().value("name").toString());
        event.eventName = v.toObject().value("name").toString();
        QString str = v.toObject().value("start-date").toString();
        QStringList strList = str.split("-");
        QDateTime startDate;
        //qDebug()<<strList;
        startDate.setDate(QDate(strList[0].toInt(),strList[1].toInt(),strList[2].toInt()));
        startDate.setTime(QTime(0, 0).addSecs(v.toObject().value("start-time").toInt()));
        //event->setStartDate(startDate);
        event.startDate = startDate;
        //qDebug()<<startDate.date();

        str = v.toObject().value("end-date").toString();
        strList = str.split("-");
        QDateTime endDate;
        endDate.setDate(QDate(strList[0].toInt(),strList[1].toInt(),strList[2].toInt()));
        endDate.setTime(QTime(0, 0).addSecs(v.toObject().value("start-time").toInt()));
        //event->setEndDate(endDate);
        event.endDate = endDate;
        //qDebug()<< eventList.length();
        //event->setDataId(eventList.length());
        event.id = eventList.length();
        eventList.append(event);
    }

    return true;
}
