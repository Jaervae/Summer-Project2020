#include "settings.h"

#include <QDebug>

Settings::Settings()
{
    QSettings settings(mOrganisation, mAppName);
    settings.beginGroup(mGroupSettings);
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
    settings.beginGroup(mGroupSettings);

    settings.setValue(key, value);
    settings.endGroup();

}

void Settings::changeSettingsInt(QString key, int value)
{
    QSettings settings(mOrganisation, mAppName);
    settings.beginGroup(mGroupData);

    settings.setValue(key, value);
    settings.endGroup();
}

QVariant Settings::loadSetting(QString key)
{
    QSettings settings(mOrganisation, mAppName);
    settings.beginGroup(mGroupSettings);

    QVariant value = settings.value(key,false).toBool();
    settings.endGroup();
    return  value;
}

QVariant Settings::loadData(QString key, QVariant datatype)
{
    QSettings settings(mOrganisation, mAppName);
    settings.beginGroup(mGroupData);
    QVariant value;

    qDebug()<<"Type is :";
    if ("int" == QLatin1String(datatype.typeName())){
        qDebug()<<"int";
        value = settings.value(key,0);
    }else if ("bool" == QLatin1String(datatype.typeName())){
        qDebug()<<"bool";
        value = settings.value(key,false);
    }else if ("double" == QLatin1String(datatype.typeName())){
        qDebug()<<"double";
        value = settings.value(key,0.0);
    }else
        qDebug()<<"unknown";


    if (key == "next-event-id"){
        int id = settings.value(key,0).toInt();
        id++;
        settings.setValue(key,id);
    }

    settings.endGroup();
    return value;
}
