#include "contact.h"

Contact::Contact(int id, QString fn, QString ln, QString mobile, QString email)
{
    this->id = id;
    this->fn = fn;
    this->ln = ln;
    this->mobile = mobile;
    this->email = email;
}

Contact::Contact(QString fn, QString ln, QString mobile, QString email)
{
    this->fn = fn;
    this->ln = ln;
    this->mobile = mobile;
    this->email = email;
}

Contact::~Contact()
{

}

int Contact::getID()
{
    return this->id;
}

QString Contact::getFN()
{
    return this->fn;
}

void Contact::setFN(QString mFN)
{
    this->fn = mFN;
}

QString Contact::getLN()
{
    return this->ln;
}

void Contact::setLN(QString mLN)
{
    this->ln = mLN;
}

QString Contact::getMobile()
{
    return this->mobile;
}

void Contact::setMobile(QString mMobile)
{
    this->mobile = mMobile;
}

QString Contact::getEmail()
{
    return this->email;
}

void Contact::setEmail(QString mEmail)
{
    this->email = mEmail;
}


