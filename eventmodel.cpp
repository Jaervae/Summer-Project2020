#include "eventmodel.h"
#include "sqleventmodel.h"

EventModel::EventModel(QObject *parent)
    : QAbstractListModel(parent)
    ,mList(nullptr)
{
}

int EventModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    return mList->items().length();
}


QVariant EventModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    const EventItem item = mList->items().at(index.row());
    switch (role) {
    case RoleID:
        return QVariant(item.id);
    case RoleEventName:
        return QVariant(item.eventName);
    case RoleStartDate:
        return QVariant(item.startDate);
    case RoleStartTime:
        return QVariant(item.startTime);
    case RoleEndDate:
        return QVariant(item.endDate);
    case RoleEndTime:
        return QVariant(item.endTime);
    }

    return QVariant();
}

bool EventModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mList)
        return false;

    EventItem item = mList->items().at(index.row());
    switch (role) {
    case RoleID:
        item.id = value.toInt();
        break;
    case RoleEventName:
        item.eventName = value.toString();
        break;
    case RoleStartDate:
        item.startDate = value.toDateTime();
        break;
    case RoleStartTime:
        item.startTime = value.toString();
        break;
    case RoleEndDate:
        item.endDate = value.toDateTime();
        break;
    case RoleEndTime:
        item.endTime = value.toString();
        break;
    }

    if (mList->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags EventModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> EventModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[RoleID] = "id";
    names[RoleEventName] = "eventName";
    names[RoleStartDate] = "startDate";
    names[RoleStartTime] = "startTime";
    names[RoleEndDate] = "endDate";
    names[RoleEndTime] = "endTime";
    return names;
}

SqlEventModel *EventModel::list() const
{
    return mList;
}

void EventModel::setList(SqlEventModel *list)
{
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList) {
        connect(mList, &SqlEventModel::preItemAppended, this, [=](int index) {
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mList, &SqlEventModel::postItemAppended, this, [=]() {
            endInsertRows();
        });


        connect(mList, &SqlEventModel::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &SqlEventModel::postItemRemoved, this, [=]() {
            endRemoveRows();
        });


    }

    endResetModel();
}
