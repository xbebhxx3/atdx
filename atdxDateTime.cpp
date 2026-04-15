//
// atdxDateTime to manipulate system clock time
//
//Created by Arvo, ES1JA 2020

#include "atdxDateTime.h"

atdxDateTime::atdxDateTime() : QDateTime {}
{}

atdxDateTime::atdxDateTime(QDateTime &&other) : QDateTime {other}
{}

atdxDateTime::atdxDateTime(const QDateTime &other) : QDateTime {other}
{}

atdxDateTime::atdxDateTime(const QDate &date, const QTime &time, const QTimeZone &timeZone) : QDateTime {date,time,timeZone}
{}

atdxDateTime::atdxDateTime(const QDate &date, const QTime &time, Qt::TimeSpec spec, int offsetSeconds) : QDateTime {date,time,spec,offsetSeconds}
{}

atdxDateTime::atdxDateTime(const QDate &date, const QTime &time, Qt::TimeSpec spec) : QDateTime {date,time,spec}
{}

qint64 atdxDateTime::currentMSecsSinceEpoch2()
  {
    return offset_ + QDateTime::currentMSecsSinceEpoch();
  }

QDateTime atdxDateTime::currentDateTimeUtc2()
  {
    return QDateTime::currentDateTimeUtc().addMSecs(offset_); //result;
  }

QDateTime atdxDateTime::currentDateTime2()
  {
    return QDateTime::currentDateTime().addMSecs(offset_); //result;
  }
