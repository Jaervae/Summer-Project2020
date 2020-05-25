#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QAbstractListModel>
#include <QObject>
#include <QVector>

struct ContactItem
{
    bool newEntry;
    QString description;
    QString mobile;
    QString email;
    int id;
};

class ContactList : public QObject
{
    Q_OBJECT
public:
    explicit ContactList(QObject *parent = nullptr);

    QVector<ContactItem> items() const;
    QVector<ContactItem> visibleItems() const;

    bool setItemAt(int index, const ContactItem &item);

signals:
    void preItemAppended(int index);
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void preItemSave();
    void postItemSave();



public slots:
    void getData();
    void appendItem();
    void removeCompletedItems();
    void removeOne(int index, bool removefromdb);
    void removeVisible(int index);
    void saveChanges(int index, QString m_desc, QString m_mobile, QString m_email, int id);
    void searchContacts(QString value);

private:
    QVector<ContactItem> mItems;
    QVector<ContactItem> mVisibleList;

    int getAlpapheticOrder(QString value);
    bool isAlphabeticallyFirst(const QString &s1, const QString &s2);
};
#endif // CONTACTLIST_H
