#ifndef atdxDATETIME_H__
#define atdxDATETIME_H__

#include <QDateTime>

//
// atdxDateTime to manipulate system clock time
//
//Created by Arvo, ES1JA 2020

class atdxDateTime : public QDateTime
{
 public:
  explicit atdxDateTime ();
  explicit atdxDateTime (QDateTime &&other);
  explicit atdxDateTime(const QDateTime &other);
  explicit atdxDateTime(const QDate &date, const QTime &time, const QTimeZone &timeZone);
  explicit atdxDateTime(const QDate &date, const QTime &time, Qt::TimeSpec spec, int offsetSeconds);
  explicit atdxDateTime(const QDate &date, const QTime &time, Qt::TimeSpec spec = Qt::LocalTime);

  QDateTime currentDateTimeUtc2();
  QDateTime currentDateTime2();
  qint64 currentMSecsSinceEpoch2();
  void SetOffset (float offset) { foffset_ = offset; offset_ = foffset_ * 1000;}
  float GetOffset () { return foffset_; }
 private:
  float foffset_ =0;
  qint64 offset_ = 0;

  using QDateTime::currentDateTimeUtc;
  using QDateTime::currentDateTime;
  using QDateTime::currentMSecsSinceEpoch;
};

#endif
