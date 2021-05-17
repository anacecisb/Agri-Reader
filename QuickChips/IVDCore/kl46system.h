#ifndef KL46SYSTEM_H
#define KL46SYSTEM_H

#include <QObject>

class KL46System : public QObject
{
    Q_OBJECT

public:
    explicit KL46System(QObject *parent = 0);
    ~KL46System();

    int id;
    int state;
    int p1;
    int fb1;

signals:

public slots:

};

#endif // KL46SYSTEM_H
