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
    /*
    mItems.append({ true, QStringLiteral("Wash the car") });
    mItems.append({ false, QStringLiteral("Fix the sink") });
    */
    for (int i = 0; i <= qList.length() ; i++){
        QString name = qList[i].getFN() + " " + qList[i].getLN();
        mItems.append({false, name});
    }
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
    if (item.done == oldItem.done && item.description == oldItem.description)
        return false;

    mItems[index] = item;
    return true;
}

void ContactList::appendItem()
{
    emit preItemAppended();

    ContactItem item;
    item.done = false;
    mItems.append(item);

    emit postItemAppended();
}

void ContactList::removeCompletedItems()
{
    for (int i = 0; i < mItems.size(); ) {
        if (mItems.at(i).done) {
            emit preItemRemoved(i);

            mItems.removeAt(i);

            emit postItemRemoved();
        } else {
            ++i;
        }
    }
}
