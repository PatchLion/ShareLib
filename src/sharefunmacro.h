#ifndef SHAREFUNMACRO
#define SHAREFUNMACRO

#include <QtCore/QTimer>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QDebug>

#define START_REPLY_TIMER(reply, timerinterval) \
    if(reply){\
        QTimer *timer = new QTimer(reply);\
        timer->setInterval(timerinterval);\
        reply->setProperty("SHARELIB_REPLY_TIMER", (long long)(timer));\
        connect(timer, &QTimer::timeout, reply, &QNetworkReply::abort);\
        timer->start();};


#define STOP_REPLY_TIMER(reply) \
    if(reply){\
    QTimer* timer = (QTimer*)(reply->property("SHARELIB_REPLY_TIMER").toLongLong());\
    if(timer){\
        timer->stop();}};
#endif // SHAREFUNMACRO

