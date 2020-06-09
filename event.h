#ifndef EVENT_H
#define EVENT_H

#include <QDateTime>
#include <QObject>
#include <QString>

class Event : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QDateTime startDate READ startDate WRITE setStartDate NOTIFY startDateChanged)
    Q_PROPERTY(QDateTime endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)
    Q_PROPERTY(int startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(int endTime READ endTime WRITE setEndTime NOTIFY endTimeChanged)


public:
    explicit Event(QObject *parent = 0);

    QString name() const;
    void setName(const QString &name);

    QDateTime startDate() const;
    void setStartDate(const QDateTime &startDate);

    QDateTime endDate() const;
    void setEndDate(const QDateTime &endDate);

    int startTime() const;
    void setStartTime(const int &startTime);

    int endTime() const;
    void setEndTime(const int &endTime);

signals:
    void nameChanged(const QString &name);
    void startDateChanged(const QDateTime &startDate);
    void endDateChanged(const QDateTime &endDate);
    void startTimeChanged(const int &startTime);
    void endTimeChanged(const int &endTime);

private:
    QString mName;
    QDateTime mStartDate;
    QDateTime mEndDate;

    int mStartTime;
    int mEndTime;
};

#endif // EVENT_H
