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
#include "sqleventmodel.h"
#include "eventmodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<ContactModel>("Contact", 1, 0, "ContactModel");
    qmlRegisterType<EventModel>("Event", 1, 0, "EventModel");
    qmlRegisterType<Settings>("Setting", 1, 0, "Settings");
    qmlRegisterUncreatableType<ContactList>("Contact", 1, 0, "ContactList",
        QStringLiteral("ContactList should not be created in QML"));
    qmlRegisterUncreatableType<SqlEventModel>("Event", 1, 0, "SqlEventModel",
        QStringLiteral("SqlEventModel should not be created in QML"));

    ContactList contactList;
    SqlEventModel sqlEventModel;
    Settings settings;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("contactList"), &contactList);
    engine.rootContext()->setContextProperty(QStringLiteral("settings"), &settings);
    engine.rootContext()->setContextProperty(QStringLiteral("sqlEventModel"), &sqlEventModel);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    return app.exec();
}

