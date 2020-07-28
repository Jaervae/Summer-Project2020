#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include <QAbstractListModel>

class SqlEventModel;

class EventModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(SqlEventModel *list READ list WRITE  setList)

public:
    explicit EventModel(QObject *parent = nullptr);

    enum {
        RoleID = Qt::UserRole,
        RoleEventName,
        RoleStartDate,
        RoleStartTime,
        RoleEndDate,
        RoleEndTime
    };


    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    SqlEventModel *list() const;
    void setList(SqlEventModel *list);

private:
    SqlEventModel *mList;
};

#endif // EVENTMODEL_H
