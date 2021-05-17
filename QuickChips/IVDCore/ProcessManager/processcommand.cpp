#include "processcommand.h"

ProcessCommand::ProcessCommand(QObject *parent) :
    QObject(parent)
{
}

QString ProcessCommand::getCommandAsString()
{
    return QString("%1,%2,%3,%4").arg(system).arg(target).arg(command).arg(value);
}
