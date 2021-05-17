#ifndef PROCESSCOMMAND_H
#define PROCESSCOMMAND_H

#include <QObject>

class ProcessCommand : public QObject
{
    Q_OBJECT
public:
    explicit ProcessCommand(QObject *parent = 0);

    QString system;
    QString target;
    QString command;
    QString value;

    QString getCommandAsString();
signals:

public slots:

};

#endif // PROCESSCOMMAND_H
