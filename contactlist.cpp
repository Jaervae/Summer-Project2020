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

bool ContactList::setItemAt(int index, const ContactItem &item)
{
    if (index < 0 || index >= mItems.size())
        return false;

    const ContactItem &oldItem = mItems.at(index);
    if (item.newEntry == oldItem.newEntry && item.description == oldItem.description)
        return false;

    mItems[index] = item;
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
    qDebug()<<"Kaikki lisatty";
}

void ContactList::appendItem()
{
    emit preItemAppended();

    ContactItem item;
    item.newEntry = true;
    mItems.append(item);

    emit postItemAppended();
}

void ContactList::removeCompletedItems()
{
    for (int i = 0; i < mItems.size(); ) {
        if (mItems.at(i).newEntry) {
            removeOne(i);
        } else {
            ++i;
        }
    }
}

void ContactList::removeOne(int index)
{
    emit preItemRemoved(index);

    FetchData *fetchdata = new FetchData();
    fetchdata->removeById(mItems[index].id);
    mItems.removeAt(index);

    emit postItemRemoved();
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
