#ifndef SQLEVENTMODEL_H
#define SQLEVENTMODEL_H

#include <QList>
#include <QObject>
#include <QDateTime>

#include "event.h"

class SqlEventModel : public QObject
{
    Q_OBJECT

public:
    SqlEventModel();
    ~SqlEventModel();

    Q_INVOKABLE QList<QObject*> eventsForDate(const QDate &date);


public slots:
    void newEvent(QString startDate, QString startTime, QString endDate, QString endTime);

private:
    void createConnection();

    bool saveChanges();
    bool loadEvents();


    QVector<Event*> eventList;

};

#endif // SQLEVENTMODEL_H
