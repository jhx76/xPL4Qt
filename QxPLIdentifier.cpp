#include "QxPLIdentifier.h"
#include <QDebug>

QxPLIdentifier::QxPLIdentifier() {
    vendorId = "";
    deviceId = "";
    instanceId = "";
}

QxPLIdentifier::QxPLIdentifier(const QString &vendorId, const QString &deviceId, const QString &instanceId) {
    this->vendorId = vendorId;
    this->deviceId = deviceId;
    this->instanceId = instanceId;
}

QxPLIdentifier::QxPLIdentifier(const QString &str)
{
    QString tmp;
    int vendorLength = str.indexOf("-");
    if(vendorLength <= 0) {
        //invalid identifier
        vendorId = "";
        deviceId = "";
        instanceId = "";
        return;
    }
    vendorId = str.left(vendorLength);
    tmp = str.right(str.length() - vendorLength - 1);
    int deviceLength = tmp.indexOf(".");
    if(deviceLength <= 0) {
        //invalid identifier
        vendorId = "";
        deviceId = "";
        instanceId = "";
        return;
    }
    deviceId = tmp.left(deviceLength);
    instanceId = tmp.right(tmp.length() - deviceLength - 1);
}

QxPLIdentifier::QxPLIdentifier(const QxPLIdentifier &other) {
    this->vendorId = other.vendorId;
    this->deviceId = other.deviceId;
    this->instanceId = other.instanceId;
}

QxPLIdentifier& QxPLIdentifier::operator =(const QxPLIdentifier& other) {
    this->vendorId = other.vendorId;
    this->deviceId = other.deviceId;
    this->instanceId = other.instanceId;
    return *this;
}

bool QxPLIdentifier::equals(const QxPLIdentifier &other) const {
    return this->vendorId == other.vendorId
            && this->deviceId == other.deviceId
            && this->instanceId == other.instanceId;
}

QxPLIdentifier QxPLIdentifier::broadcast() {
    return QxPLIdentifier("*", "", "");
}


bool QxPLIdentifier::isBroadcast() const {
    return this->vendorId == "*" && deviceId.isEmpty() && instanceId.isEmpty();
}

bool operator==(const QxPLIdentifier& a, const QxPLIdentifier& b) {
    return a.equals(b);
}

bool operator!=(const QxPLIdentifier& a, const QxPLIdentifier& b) {
    return !a.equals(b);
}

QString QxPLIdentifier::toString() const {
    if(isBroadcast())
        return "*";
    return vendorId+"-"+deviceId+"."+instanceId;
}

bool QxPLIdentifier::isValid() const {
    return !(vendorId.isEmpty() && deviceId.isEmpty());
}
