#ifndef NOTIFICATIONCLIENT_H
#define NOTIFICATIONCLIENT_H


#include <QObject>

class NotificationClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString notification READ notification WRITE setNotification NOTIFY notificationChanged)
    Q_PROPERTY(QString date READ date WRITE setDate)
    Q_PROPERTY(QString time READ time WRITE setTime)

public:
    explicit NotificationClient(QObject *parent = 0);

    void setNotification(const QString &notification);
    QString notification() const;

    void setDate(const QString &date);
    QString date() const;

    void setTime(const QString &time);
    QString time() const;

signals:
    void notificationChanged();


private slots:
    void updateAndroidNotification();

private:
    QString m_notification;
    QString m_date;
    QString m_time;
};

#endif // NOTIFICATIONCLIENT_H
