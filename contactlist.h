#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QObject>
#include <QVector>

struct ContactItem
{
    bool newEntry;
    QString description;
    QString mobile;
    QString email;
};

class ContactList : public QObject
{
    Q_OBJECT
public:
    explicit ContactList(QObject *parent = nullptr);

    QVector<ContactItem> items() const;

    bool setItemAt(int index, const ContactItem &item);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void preItemSave();
    void postItemSave();

public slots:
    void appendItem();
    void removeCompletedItems();
    void removeOne(int index);
    void saveChanges(int index, QString m_desc, QString m_mobile, QString m_email);

private:
    QVector<ContactItem> mItems;
};
#endif // CONTACTLIST_H
