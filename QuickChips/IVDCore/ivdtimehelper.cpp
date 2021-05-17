#include "ivdtimehelper.h"

IVDTimeHelper::IVDTimeHelper()
{
}


QString IVDTimeHelper::getDateYMDFormatA()
{
    QDate nowDate = QDate::currentDate();
    QString date = QString( "[%1.%2.%3]" ).arg(nowDate.year()).arg(nowDate.month()).arg(nowDate.day());
    return date;
}

QString IVDTimeHelper::getTimeHMSFormatA()
{
    QTime now = QTime::currentTime();
    QString time = QString( "[%1:%2:%3]" ).arg(now.hour()).arg(now.minute()).arg(now.second());
    return time;
}
