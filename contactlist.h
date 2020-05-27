#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QAbstractListModel>
#include <QObject>
#include <QVector>
#include <QFile>
#include <QFileDialog>

struct ContactItem
{
    bool newEntry;
    QString fullname;
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
    void saveChanges(int index, QString m_fullname, QString m_mobile, QString m_email, int id);
    void searchContacts(QString value);

    void listToJsonArray();
    bool saveToFile();
    bool loadList();

private:
    QVector<ContactItem> mItems;
    QVector<ContactItem> mVisibleList;

    QString filename;

    int getAlpapheticOrder(QString value);
    bool isAlphabeticallyFirst(const QString &s1, const QString &s2);
};
#endif // CONTACTLIST_H
