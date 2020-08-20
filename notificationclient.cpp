#include "notificationclient.h"

#include <QtAndroid>

NotificationClient::NotificationClient(QObject *parent)
    : QObject(parent)
{
    connect(this, SIGNAL(notificationChanged()), this, SLOT(updateAndroidNotification()));
}

void NotificationClient::setNotification(const QString &notification)
{
    if (m_notification == notification)
        return;

    m_notification = notification;
    emit notificationChanged();
}

QString NotificationClient::notification() const
{
    return m_notification;
}

void NotificationClient::setDate(const QString &date)
{
    if (m_date == date)
        return;

    m_date = date;
}

QString NotificationClient::date() const
{
    return m_date;
}

void NotificationClient::setTime(const QString &time)
{
    if (m_time == time)
        return;

    m_time = time;
}

QString NotificationClient::time() const
{
    return m_time;
}


void NotificationClient::updateAndroidNotification()
{
    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(m_notification);
    QAndroidJniObject javaDate = QAndroidJniObject::fromString(m_date);
    QAndroidJniObject javaTime = QAndroidJniObject::fromString(m_time);
    QAndroidJniObject::callStaticMethod<void>(
        "com/qtproject/example/notification/NotificationClient",
        "notify",
        "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",
        QtAndroid::androidContext().object(),
        javaNotification.object<jstring>(),
        javaDate.object<jstring>(),
        javaTime.object<jstring>());
}
