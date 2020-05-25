#include <QCoreApplication>
#include <QTime>

#include "contactlist.h"
#include "fetchdata.h"
#include "contact.h"

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
    if (item.newEntry == oldItem.newEntry && item.description == oldItem.description)
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
    }
    mVisibleList = mItems;
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


void ContactList::saveChanges(int index, QString m_desc, QString m_mobile, QString m_email, int id)
{
   emit preItemSave();

   QStringList fullname = m_desc.split(' ');
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
   fetchdata->getNewEntryID(m_desc);
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
        if(mItems[i].description.contains(value,Qt::CaseInsensitive)) {
            bool alreadyInList = false;
            for(int j = 0; j < mVisibleList.length(); ++j){
                if(mItems[i].description == mVisibleList[j].description){
                    ///Already in list
                    alreadyInList = true;
                    break;
                }
            }
            if(!alreadyInList){
                int index = getAlpapheticOrder(mItems[i].description);

                emit preItemAppended(index);

                mVisibleList.insert(index, mItems[i]);
                //mVisibleList.append(mItems[i]);

                emit postItemAppended();
            }
        }
        //if not
        else {
            for(int j = 0; j < mVisibleList.size(); ){
                if(mItems.at(i).description == mVisibleList.at(j).description){
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
        qDebug()<<mVisibleList[i].description;
    }
    qDebug()<<mVisibleList.length();
    qDebug()<< "-----------------------------";

}

int ContactList::getAlpapheticOrder(QString value)
{
    int index = mVisibleList.length();
    bool spotFound = false;
    for (int i = 0; i < mVisibleList.length(); i++){
        if(isAlphabeticallyFirst(value, mVisibleList[i].description)){
            index = i;
            spotFound = true;
            break;
        }
    }
    return index;
}

bool ContactList::isAlphabeticallyFirst(const QString &s1, const QString &s2)
{
    return s1.toLower() < s2.toLower();
}
