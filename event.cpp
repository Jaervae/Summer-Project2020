#include "event.h"

Event::Event(QObject *parent) :
    QObject(parent)
{
}

QString Event::name() const
{
    return mName;
}

void Event::setName(const QString &name)
{
    if (name != mName) {
        mName = name;
        emit nameChanged(mName);
    }
}

QDateTime Event::startDate() const
{
    return mStartDate;
}

void Event::setStartDate(const QDateTime &startDate)
{
    if (startDate != mStartDate) {
        mStartDate = startDate;
        emit startDateChanged(mStartDate);
    }
}

QDateTime Event::endDate() const
{
    return mEndDate;
}

void Event::setEndDate(const QDateTime &endDate)
{
    if (endDate != mEndDate) {
        mEndDate = endDate;
        emit endDateChanged(mEndDate);
    }
}

int Event::startTime() const
{
    return mStartTime;
}

void Event::setStartTime(const int &startTime)
{
    if (startTime != mStartTime){
        mStartTime = startTime;
        emit startTimeChanged(mStartTime);
    }
}

int Event::endTime() const
{
    return mEndTime;
}

void Event::setEndTime(const int &endTime)
{
    if (endTime != mEndTime){
        mEndTime = endTime;
        emit endTimeChanged(mEndTime);
    }
}
