#ifndef QXPLDEVICE_H
#define QXPLDEVICE_H

#include "QxPLIdentifier.h"

class QxPLDevice
{
private:
    QxPLIdentifier identifier;

public:
    QxPLDevice(const QxPLIdentifier& identifier);

    const QxPLIdentifier& getIdentifier() const { return identifier; }
    void setIdentifier(const QxPLIdentifier& identifier) { this->identifier = identifier; }

};

#endif // QXPLDEVICE_H
