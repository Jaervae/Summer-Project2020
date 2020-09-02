#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <iostream>
#include <QQmlContext>
#include <QStandardPaths>

#include "settings.h"
#include "fetchdata.h"
#include "contactlist.h"
#include "contactmodel.h"
#include "sqlevent.h"
#include "eventmodel.h"
#include "selecteddayeventsmodel.h"
#include "notificationclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<ContactModel>("Contact", 1, 0, "ContactModel");
    qmlRegisterType<EventModel>("Event", 1, 0, "EventModel");
    qmlRegisterType<SelectedDayEventsModel>("CurrentDayEvents", 1, 0, "CurrentDayEventsModel");
    qmlRegisterType<Settings>("Setting", 1, 0, "Settings");

    qmlRegisterUncreatableType<ContactList>("Contact", 1, 0, "ContactList",
        QStringLiteral("ContactList should not be created in QML"));
    qmlRegisterUncreatableType<SqlEvent>("Event", 1, 0, "SqlEvent",
        QStringLiteral("SqlEvent should not be created in QML"));


    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    ContactList contactList;
    SqlEvent sqlEvent;
    Settings settings;

    NotificationClient *notificationClient = new NotificationClient(&engine);

    engine.rootContext()->setContextProperty(QStringLiteral("contactList"), &contactList);
    engine.rootContext()->setContextProperty(QStringLiteral("settings"), &settings);
    engine.rootContext()->setContextProperty(QStringLiteral("sqlEvent"), &sqlEvent);
    engine.rootContext()->setContextProperty(QLatin1String("notificationClient"), notificationClient);


    return app.exec();
}

