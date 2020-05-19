#include "contactmodel.h"
#include "contactlist.h"

ContactModel::ContactModel(QObject *parent)
    : QAbstractListModel(parent)
    , mVisibleList(nullptr)
{
}

int ContactModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mVisibleList)
        return 0;

    return mVisibleList->visibleItems().size();
}

QVariant ContactModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mVisibleList)
        return QVariant();

    const ContactItem item = mVisibleList->visibleItems().at(index.row());
    switch (role) {
    case NewEntryRole:
        return QVariant(item.newEntry);
    case DescriptionRole:
        return QVariant(item.description);
    case MobileRole:
        return QVariant(item.mobile);
    case EmailRole:
        return QVariant(item.email);
    case IDRole:
        return QVariant(item.id);
    }

    return QVariant();
}

bool ContactModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mVisibleList)
        return false;

    ContactItem item = mVisibleList->visibleItems().at(index.row());
    switch (role) {
    case NewEntryRole:
        item.newEntry = value.toBool();
        break;
    case DescriptionRole:
        item.description = value.toString();
        break;
    case MobileRole:
        item.mobile = value.toString();
        break;
    case EmailRole:
        item.email = value.toString();
        break;
    case IDRole:
        item.id = value.toInt();
        break;
    }

    if (mVisibleList->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ContactModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> ContactModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NewEntryRole] = "newEntry";
    names[DescriptionRole] = "description";
    names[MobileRole] = "mobile";
    names[EmailRole] = "email";
    names[IDRole] = "id";
    return names;
}

ContactList *ContactModel::list() const
{
    return mVisibleList;
}

void ContactModel::setList(ContactList *list)
{
    beginResetModel();

    if (mVisibleList)
        mVisibleList->disconnect(this);

    mVisibleList = list;

    if (mVisibleList) {
        connect(mVisibleList, &ContactList::preItemAppended, this, [=]() {
            const int index = mVisibleList->visibleItems().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mVisibleList, &ContactList::postItemAppended, this, [=]() {
            endInsertRows();
        });


        connect(mVisibleList, &ContactList::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mVisibleList, &ContactList::postItemRemoved, this, [=]() {
            endRemoveRows();
        });


    }

    endResetModel();
}
