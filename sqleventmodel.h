#ifndef SQLEVENTMODEL_H
#define SQLEVENTMODEL_H

#include <QList>
#include <QObject>

#include "event.h"

class SqlEventModel : public QObject
{
    Q_OBJECT

public:
    SqlEventModel();

    Q_INVOKABLE QList<QObject*> eventsForDate(const QDate &date);

private:
    void createConnection();

    bool saveChanges();
    bool loadEvents();

    QVector<Event*> eventList;

};

#endif // SQLEVENTMODEL_H
