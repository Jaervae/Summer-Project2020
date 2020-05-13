#include "contactmodel.h"
#include "contactlist.h"

ContactModel::ContactModel(QObject *parent)
    : QAbstractListModel(parent)
    , mList(nullptr)
{
}

int ContactModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    return mList->items().size();
}

QVariant ContactModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    const ContactItem item = mList->items().at(index.row());
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
    if (!mList)
        return false;

    ContactItem item = mList->items().at(index.row());
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

    if (mList->setItemAt(index.row(), item)) {
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
    return mList;
}

void ContactModel::setList(ContactList *list)
{
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList) {
        connect(mList, &ContactList::preItemAppended, this, [=]() {
            const int index = mList->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mList, &ContactList::postItemAppended, this, [=]() {
            endInsertRows();
        });

        connect(mList, &ContactList::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &ContactList::postItemRemoved, this, [=]() {
            endRemoveRows();
        });
    }

    endResetModel();
}
