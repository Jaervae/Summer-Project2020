#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QAbstractListModel>

class ContactList;

class ContactModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ContactList *list READ list WRITE setList)

public:
    explicit ContactModel(QObject *parent = nullptr);

    enum {
            NewEntryRole = Qt::UserRole,
            DescriptionRole,
            MobileRole,
            EmailRole,
            IDRole
        };

        // Basic functionality:
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        // Editable:
        bool setData(const QModelIndex &index, const QVariant &value,
                     int role = Qt::EditRole) override;

        Qt::ItemFlags flags(const QModelIndex& index) const override;

        virtual QHash<int, QByteArray> roleNames() const override;

        ContactList *list() const;
        void setList(ContactList *list);

    private:
        ContactList *mList;
    };

#endif // CONTACTLISTMODEL_H
