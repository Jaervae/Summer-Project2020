#include <QCoreApplication>
#include <QTime>

#include "contactlist.h"
#include "fetchdata.h"
#include "contact.h"

ContactList::ContactList(QObject *parent) : QObject(parent)
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

    for (int i = 0; i < qList.length() ; i++){
        QString name = qList[i].getFN() + " " + qList[i].getLN();
        QString mobile = qList[i].getMobile();
        QString email = qList[i].getEmail();
        mItems.append({false, name, mobile, email});
        qDebug()<<"Lisätty: " + name;
    }
    qDebug()<<"Kaikki lisatty";
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

    mItems.removeAt(index);

    emit postItemRemoved();
}

void ContactList::saveChanges(int index, QString m_desc, QString m_mobile, QString m_email)
{
   emit preItemSave();

   QStringList fullname = m_desc.split(' ');
   QString ln = fullname.last();
   fullname.removeLast();
   QString fn = fullname.join(' ');
   QString mobile = m_mobile;
   QString email = m_email;

   Contact contact = Contact(fn,ln,mobile,email);

   FetchData *fetchdata = new FetchData();
   fetchdata->putData(contact, mItems[index].newEntry);

   mItems[index].newEntry = false;

   emit postItemSave();
}
