#include <QCoreApplication>
#include <QTime>

#include "contactlist.h"
#include "fetchdata.h"
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
    this->listToJsonArray();
    loadList();
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

    FetchData *fetchdata = new FetchData();
    if (removefromdb)fetchdata->removeById(mItems[index].id);
    removeVisible(mItems[index].id);
    mItems.removeAt(index);


}

void ContactList::removeVisible(int index)
{
    for(int i = 0; i < mVisibleList.size(); i++){
        if(mVisibleList[i].id == index){
            emit preItemRemoved(i);
            mVisibleList.removeAt(i);
            emit postItemRemoved();
            break;
        }
    }

}


void ContactList::saveChanges(int index, QString m_fullname, QString m_mobile, QString m_email, int id)
{
   emit preItemSave();

   QStringList fullname = m_fullname.split(' ');
   QString ln;
   if(fullname.length() > 1){
       ln = fullname.last();
       fullname.removeLast();
   } else ln = "";
   QString fn = fullname.join(' ');

   Contact contact = Contact(id, fn,ln,m_mobile,m_email);

   FetchData *fetchdata = new FetchData();
   fetchdata->putData(contact, mItems[index].newEntry);

   mItems[index].newEntry = false;
   fetchdata->getNewEntryID(m_fullname);
   while (fetchdata->getSearchStatus() == false) {
       QTime dieTime= QTime::currentTime().addMSecs(200);
       while (QTime::currentTime() < dieTime)
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
   }
   mItems[index].id = fetchdata->getWantedID();
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

bool ContactList::saveToFile()
{


    return true;
}

void ContactList::listToJsonArray()
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

    saveToFile();
}

bool ContactList::loadList()
{
    QFile file(QStringLiteral("save.json"));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = jsonResponse.array();
    qDebug()<<jsonArray;
    /*QJsonDocument document = QJsonDocument::fromJson(jsonData);
    QJsonObject object = document.object();*/
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
