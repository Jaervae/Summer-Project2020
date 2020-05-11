#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QObject>
#include <QVector>

struct ContactItem
{
    bool done;
    QString description;
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

public slots:
    void appendItem();
    void removeCompletedItems();

private:
    QVector<ContactItem> mItems;
};
#endif // CONTACTLIST_H
