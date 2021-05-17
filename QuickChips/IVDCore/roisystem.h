#ifndef ROISYSTEM_H
#define ROISYSTEM_H

#include "roi.h"

#include <QObject>


//enum ROISystemStyle
//{

//};

class ROISystem : public QObject
{
    Q_OBJECT
public:
    explicit ROISystem(QObject *parent = 0);

    QString name;
    QList<ROI*> signalROIs;
    QList<ROI*> backgroundROIs;


signals:

public slots:

};

#endif // ROISYSTEM_H
