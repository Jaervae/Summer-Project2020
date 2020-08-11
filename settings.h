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
    QString getSettingGroup(){return mGroupSettings;}
    QString getDataGroup(){return mGroupData;}
    QString getNextEventId(){return mNextEventId;}


public slots:
    void saveSettings();
    void loadSettings();
    void changeSettingsBool(QString key, bool value);
    void changeSettingsInt (QString key, int value);
    QVariant loadSetting(QString key);
    QVariant loadData(QString key, QVariant datatype);


private:
    QString mOrganisation = "MyImagineOrg";
    QString mAppName = "UtilityApp";
    QString mGroupSettings = "Settings";
    QString mGroupData = "DataGroup";

    QString mNextEventId = "NextEventId";
};

#endif // SETTINGS_H
