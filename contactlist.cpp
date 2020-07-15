#include <QCoreApplication>
#include <QTime>

#include "contactlist.h"
#include "fetchdata.h"
#include "settings.h"
#include "contact.h"
#include <QNetworkReply>
#include <QtNetwork>
#include <QNetworkAccessManager>

ContactList::ContactList(QObject *parent) : QObject(parent)
{
    this->getData();
}

QVector<ContactItem> ContactList::items() const
{
    return mItems;
}

QVector<ContactItem> ContactList::visibleItems() const
{
    return mVisibleList;
}

bool ContactList::setItemAt(int index, const ContactItem &item)
{
    if (index < 0 || index >= mVisibleList.size())
        return false;

    const ContactItem &oldItem = items().at(index);
    if (item.newEntry == oldItem.newEntry && item.fullname == oldItem.fullname)
        return false;

    mVisibleList[index] = item;
    return true;
}

void ContactList::settingsChanged()
{
    qDebug()<< "Settings changed";
}

void ContactList::getData()
{
    FetchData *fetchdata = new FetchData();
    fetchdata->getData();
    while (fetchdata->getSearchStatus() == false) {
        QTime dieTime= QTime::currentTime().addSecs(1);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        qDebug()<< "Search is not done";
    }
    QList<Contact> qList = fetchdata->getList();
    qDebug()<< "Search done, list size:";
    qDebug()<< qList.length();

    if(mItems.length() >= 0) mItems.clear();
    for (int i = 0; i < qList.length() ; i++){
        QString name = qList[i].getFN() + " " + qList[i].getLN();
        QString mobile = qList[i].getMobile();
        QString email = qList[i].getEmail();
        int id = qList[i].getID();

        mItems.append({false, name, mobile, email,id});
        qDebug()<<"Lisätty: " + name;

        int index = getAlpapheticOrder(mItems[i].fullname);

        mVisibleList.insert(index, mItems[i]);
    }
    qDebug()<<"Kaikki lisatty";

}

void ContactList::appendItem()
{
    emit preItemAppended(mVisibleList.length());

    ContactItem item;

    item.newEntry = true;
    mItems.append(item);
    mVisibleList.append(item);

    emit postItemAppended();
}


void ContactList::removeCompletedItems()
{
    for (int i = 0; i < mItems.size(); ) {
        if (mItems.at(i).newEntry) {
            removeOne(i,false);
        } else {
            ++i;
        }
    }

}

void ContactList::removeOne(int index, bool removefromdb)
{
    int id = mVisibleList[index].id;

    emit preItemRemoved(index);
    mVisibleList.removeAt(index);
    emit postItemRemoved();

    for(int i = 0; i < mItems.size(); i++){
        if(mItems[i].id == id){
            FetchData *fetchdata = new FetchData();
            if (removefromdb)fetchdata->removeById(mItems[i].id);
            mItems.removeAt(i);
            break;
        }
    }
}



void ContactList::saveChanges(int indexOfVisibleList, QString m_fullname, QString m_mobile, QString m_email, int itemId)
{

   emit preItemSave();

   QStringList fullname = m_fullname.split(' ');
   QString ln;
   if(fullname.length() > 1){
       ln = fullname.last();
       fullname.removeLast();
   } else ln = "";
   QString fn = fullname.join(' ');

   Contact contact = Contact(itemId, fn,ln,m_mobile,m_email);

   Settings *settingsObj = new Settings();
   bool syncEnabled = settingsObj->loadSetting("sync-enabled").toBool();
   int indexOfmItems;

   for(int i = 0; i < mItems.size(); i++){
       if(mVisibleList[indexOfVisibleList].id == mItems[i].id) {
           indexOfmItems = i;
           break;
       }

   }

   if (syncEnabled){
       FetchData *fetchdata = new FetchData();
       fetchdata->putData(contact, mVisibleList[indexOfVisibleList].newEntry);

       fetchdata->getNewEntryID(m_fullname);
       while (fetchdata->getSearchStatus() == false) {
           QTime dieTime= QTime::currentTime().addMSecs(200);
           while (QTime::currentTime() < dieTime)
               QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
       }
       QStringList itemObj = { fn, ln , m_mobile, m_email };
       //mItems[indexOfmItems].id = fetchdata->getWantedID();



   }
   FetchData *fetchdata = new FetchData();
   fetchdata->getData();
   while (fetchdata->getSearchStatus() == false) {
       QTime dieTime= QTime::currentTime().addSecs(1);
       while (QTime::currentTime() < dieTime)
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
       qDebug()<< "Search is not done";
   }
   QList<Contact> qList = fetchdata->getList();
   for(int i = 0; i < qList.size(); i++){
       if(mVisibleList[indexOfVisibleList].fullname == qList[i].getFN() + " " + qList[i].getLN() && mVisibleList[indexOfVisibleList].email == qList[i].getEmail()) {
           mVisibleList[indexOfVisibleList].id = qList[i].getID();
           mItems[indexOfmItems].id = qList[i].getID();
           break;
       }
    mItems[indexOfmItems].newEntry = false;
   }

   mItems[indexOfmItems].fullname = mVisibleList[indexOfVisibleList].fullname;
   qDebug()<<mItems[indexOfmItems].fullname;
   qDebug()<<mVisibleList[indexOfVisibleList].fullname;
   qDebug()<<mItems[indexOfmItems].id;
   qDebug()<<mVisibleList[indexOfVisibleList].id;
   qDebug()<<mItems.length();
   qDebug()<<mVisibleList.length();
   qDebug()<<mItems[indexOfmItems].newEntry;
   qDebug()<<mVisibleList[indexOfVisibleList].newEntry;
   emit postItemSave();
}

void ContactList::searchContacts(QString value)
{
    for (int i = 0; i < mItems.length();++i) {
        //Checks if typed value mathces any items on list
        //if true
        if(mItems[i].fullname.contains(value,Qt::CaseInsensitive)) {
            bool alreadyInList = false;
            for(int j = 0; j < mVisibleList.length(); ++j){
                if(mItems[i].fullname == mVisibleList[j].fullname){
                    ///Already in list
                    alreadyInList = true;
                    break;
                }
            }
            if(!alreadyInList){
                int index = getAlpapheticOrder(mItems[i].fullname);

                emit preItemAppended(index);

                mVisibleList.insert(index, mItems[i]);
                //mVisibleList.append(mItems[i]);

                emit postItemAppended();
            }
        }
        //if not
        else {
            for(int j = 0; j < mVisibleList.size(); ){
                if(mItems.at(i).fullname == mVisibleList.at(j).fullname){
                    emit preItemRemoved(j);

                    mVisibleList.removeAt(j);

                    emit postItemRemoved();
                    break;
                }else
                    ++j;
            }
        }
    }
    for (int i = 0; i < mVisibleList.length();++i) {
        qDebug()<<mVisibleList[i].fullname;
    }
    qDebug()<<mVisibleList.length();
    qDebug()<< "-----------------------------";

}

void ContactList::saveToFile()
{
    QJsonArray array;
    foreach(const ContactItem contact,mItems){
        QStringList fullnameAsList = contact.fullname.split(' ');
        QString ln;
        if(fullnameAsList.length() > 1){
            ln = fullnameAsList.last();
            fullnameAsList.removeLast();
        } else ln = "";
        QString fn = fullnameAsList.join(' ');

        QJsonObject contactObj;
        contactObj["id"] = contact.id;
        contactObj["fistname"] = fn;
        contactObj["lastname"] = ln;
        contactObj["mobile"] = contact.mobile;
        contactObj["email"] = contact.email;
       array.append(contactObj);
    }
    QJsonObject object;
    object["Contacts"] = array;
    QJsonDocument saveDoc(object);
    //qDebug()<<array;

    QFile saveFile(QStringLiteral("save.json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");

    }
    saveFile.write(QJsonDocument(array).toJson());

    qDebug()<<"done saving";
}

bool ContactList::loadList()
{   /* Old one
    QFile file(QStringLiteral("save.json"));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = jsonResponse.array();
    */

    Settings *settingsObj = new Settings();
    bool syncCloud = settingsObj->loadSetting("sync-cloud").toBool();

    if(syncCloud == true){
        qDebug()<< "Syncing from cloud";
        while (mItems.size() > 0) {
            removeOne(0,false);
        }
        FetchData *fetchdata = new FetchData();
        fetchdata->getData();
        while (fetchdata->getSearchStatus() == false) {
            QTime dieTime= QTime::currentTime().addMSecs(200);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
        }
        QList<Contact> qList = fetchdata->getList();
        for(int i = 0; i < qList.size(); i++){
            emit preItemAppended(mVisibleList.length());

            ContactItem item;
            item.fullname = qList[i].getFN() + " " + qList[i].getLN();
            item.email = qList[i].getEmail();
            item.mobile = qList[i].getMobile();
            item.newEntry = false;
            item.id = qList[i].getID();
            mItems.append(item);
            int index = getAlpapheticOrder(item.fullname);
            mVisibleList.insert(index, item);
            emit postItemAppended();
        }
        qDebug()<< mItems.length();
        this->saveToFile();
    }else{
        qDebug()<< "Syncing from local";
        FetchData *fetchdata = new FetchData();
        fetchdata->getData();
        while (fetchdata->getSearchStatus() == false) {
            QTime dieTime= QTime::currentTime().addMSecs(200);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
        }
        QList<Contact> qList = fetchdata->getList();
        for (int i = 0; i < mVisibleList.length(); i++){
            bool alreadyOnCloud = false;
            for(int j = 0; j < qList.length(); j++){
                /*qDebug()<< mItems[i].id;
                qDebug()<< qList[j].getID();
                qDebug()<< "---------------";*/
                if(mVisibleList[i].id == qList[j].getID()){
                    qDebug()<<"found: " + qList[j].getFN() + " || " + mVisibleList[i].fullname;
                    if(mVisibleList[i].fullname != qList[j].getFN() + " " + qList[j].getLN() || mVisibleList[i].email != qList[j].getEmail()|| mVisibleList[i].mobile != qList[j].getMobile()){
                        this->saveChanges(i, mVisibleList[i].fullname, mVisibleList[i].mobile, mVisibleList[i].email, mVisibleList[i].id);
                        qDebug()<<"Diffrence found from:" + mVisibleList[i].fullname + ", " +qList[j].getFN() + " " + qList[j].getLN();
                        qDebug()<< mVisibleList[i].email + ", " +qList[j].getEmail();
                        qDebug()<< mVisibleList[i].mobile + ", " +qList[j].getMobile();
                    }
                    alreadyOnCloud = true;
                    break;
                }
            }
            if (!alreadyOnCloud){
                qDebug()<< "new entry: " + mVisibleList[i].fullname;
                qDebug()<< mVisibleList[i].id;
                this->saveChanges(i, mVisibleList[i].fullname, mVisibleList[i].mobile, mVisibleList[i].email, mVisibleList[i].id);
            }
        }

    }

    return true;
}



int ContactList::getAlpapheticOrder(QString value)
{
    int index = mVisibleList.length();
    for (int i = 0; i < mVisibleList.length(); i++){
        if(isAlphabeticallyFirst(value, mVisibleList[i].fullname)){
            index = i;
            break;
        }
    }
    return index;
}

bool ContactList::isAlphabeticallyFirst(const QString &s1, const QString &s2)
{
    return s1.toLower() < s2.toLower();
}
