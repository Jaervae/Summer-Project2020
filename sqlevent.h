#ifndef SQLEVENTMODEL_H
#define SQLEVENTMODEL_H

#include <QList>
#include <QObject>
#include <QDateTime>
#include <QAbstractListModel>
#include <QVector>
#include <QFile>
#include <QFileDialog>


struct EventItem{
    int id;
    QString eventName;
    QDateTime startDate;
    QString startTime;
    QDateTime endDate;
    QString endTime;
};

class SqlEvent : public QObject
{
    Q_OBJECT

public:
    explicit SqlEvent(QObject *parent = nullptr);

    Q_INVOKABLE QList<QObject*> eventsForDate(const QDate &date);
    Q_INVOKABLE void getCurrentEventsForSelectedDate(const QDate &date);

    QVector<EventItem> items() const;
    QVector<EventItem> currentItems() const;

    bool setItemAt(int index, const EventItem &item);
    bool setItemAtSelectedList(int index, const EventItem &item);

signals:
    void preItemAppended(int index);
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void preItemSave();
    void postItemSave();

public slots:
    void newEvent(QString eventName, QString startDate, QString startTime, QString endDate, QString endTime);
    void appendItem(EventItem item);
    void removeOne(int index);


private:
    void createConnection();

    bool saveChanges();
    bool loadEvents();

    QVector<EventItem> eventList;
    QVector<EventItem> selectedDateEventsList;

    QString filename;

};

#endif // SQLEVENTMODEL_H
