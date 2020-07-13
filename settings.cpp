#include "settings.h"

Settings::Settings()
{
    QSettings settings(mOrganisation, mAppName);
    settings.beginGroup(mGropuSettings);
    if(settings.value("fistlaunch",true).toBool() == true){
         settings.setValue("sync-cloud", true);
         settings.setValue("sync-enabled",true);
         settings.setValue("fistlaunch",false);
    }
    settings.endGroup();
}

void Settings::saveSettings()
{

}

void Settings::loadSettings()
{

}

void Settings::changeSettingsBool(QString key, bool value)
{
    QSettings settings(mOrganisation, mAppName);
    settings.beginGroup(mGropuSettings);

    settings.setValue(key, value);
    settings.endGroup();

}

QVariant Settings::loadSetting(QString key)
{

    QSettings settings(mOrganisation, mAppName);
    settings.beginGroup(mGropuSettings);

    QVariant value = settings.value(key,false).toBool();
    settings.endGroup();

    return  value;
}
