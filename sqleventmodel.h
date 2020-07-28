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

class SqlEventModel : public QObject
{
    Q_OBJECT

public:
    explicit SqlEventModel(QObject *parent = nullptr);

    Q_INVOKABLE QList<QObject*> eventsForDate(const QDate &date);


    QVector<EventItem> items() const;

    bool setItemAt(int index, const EventItem &item);

signals:
    void preItemAppended(int index);
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void preItemSave();
    void postItemSave();

public slots:
    void newEvent(QString eventName, QString startDate, QString startTime, QString endDate, QString endTime);
    void removeOne(int index);
    QList<EventItem> eventsForDateE(const QDate &date);

private:
    void createConnection();

    bool saveChanges();
    bool loadEvents();


    QVector<EventItem> eventList;
    QString filename;

};

#endif // SQLEVENTMODEL_H
