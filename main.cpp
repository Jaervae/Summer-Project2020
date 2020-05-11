#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <iostream>
#include <QQmlContext>

#include "fetchdata.h"
#include "contactlist.h"
#include "contactmodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<ContactModel>("Contact", 1, 0, "ContactModel");
    qmlRegisterUncreatableType<ContactList>("Contact", 1, 0, "ContactList",
        QStringLiteral("ContactList should not be created in QML"));

    ContactList contactList;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("contactList"), &contactList);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    return app.exec();
}

