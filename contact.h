#ifndef CONTACT_H
#define CONTACT_H

#include <qstring.h>




class Contact
{
public:
    Contact(int id, QString fn, QString ln, QString mobile, QString email);
    Contact(QString fn, QString ln, QString mobile, QString email);
    ~Contact();

   int getID();

   QString getFN();
   void setFN(QString mFN);

   QString getLN();
   void setLN(QString mLN);

   QString getMobile();
   void setMobile(QString mMobile);

   QString getEmail();
   void setEmail(QString mEmail);

private:
   int id;
   QString fn;
   QString ln;
   QString mobile;
   QString email;
};

#endif // CONTACT_H
