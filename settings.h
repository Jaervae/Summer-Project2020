#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT

public:
    Settings();

    bool checkSettingStatus();

    QString getOrganisation(){return mOrganisation;}
    QString getAppName(){return mAppName;}
    QString getSettingGroup(){return mGropuSettings;}

public slots:
    void saveSettings();
    void loadSettings();
    void changeSettingsBool(QString key, bool value);
    QVariant loadSetting(QString key);


private:
    QString mOrganisation = "MyImagineOrg";
    QString mAppName = "UtilityApp";
    QString mGropuSettings = "Settings";
};

#endif // SETTINGS_H
