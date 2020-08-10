#ifndef SELECTEDDAYEVENTSMODEL_H
#define SELECTEDDAYEVENTSMODEL_H

#include <QAbstractListModel>

class SqlEvent;

class SelectedDayEventsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(SqlEvent *list READ list WRITE  setList)

public:
    explicit SelectedDayEventsModel (QObject *parent = nullptr);

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

    SqlEvent *list() const;
    void setList(SqlEvent *list);

private:
    SqlEvent *mList;
};

#endif // SELECTEDDAYEVENTSMODEL_H
