QT += quick
QT += network
QT += core
QT += quickcontrols2
QT += widgets
QT += qml quick sql
QT += androidextras


CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        contact.cpp \
        contactlist.cpp \
        contactmodel.cpp \
        event.cpp \
        eventmodel.cpp \
        fetchdata.cpp \
        main.cpp \
        notificationclient.cpp \
        selecteddayeventsmodel.cpp \
        settings.cpp \
        sqlevent.cpp

RESOURCES += qml.qrc \
    images.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
android: include(C:/Users/ollij/Android/sdk/android_openssl/openssl.pri)

HEADERS += \
    contact.h \
    contactlist.h \
    contactmodel.h \
    event.h \
    eventmodel.h \
    fetchdata.h \
    notificationclient.h \
    selecteddayeventsmodel.h \
    settings.h \
    sqlevent.h

DISTFILES += \ \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/src/com/qtproject/example/notification/MyNotificationPublisher.java

android {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    DISTFILES += \
        android/AndroidManifest.xml \
        android/src/com/qtproject/example/notification/NotificationClient.java \
        android/build.gradle \
        android/res/values/libs.xml
}
