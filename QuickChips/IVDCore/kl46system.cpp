#include "kl46system.h"

KL46System::KL46System(QObject *parent) :
    QObject(parent)
{
    id = -1;
    state = -1;
    p1 = -1;
    fb1 = -1;
}


KL46System::~KL46System()
{

}
