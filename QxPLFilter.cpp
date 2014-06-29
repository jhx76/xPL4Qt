#include "QxPLFilter.h"

QxPLFilter::QxPLFilter() : QxPLMessage()
{
    setHop(0);
}

bool QxPLFilter::match(const QxPLMessage &message) const {
    //compare the types
    if(this->getType() != ANY && message.getType() != this->getType())
        return false;

    //compare hop counts
    if(this->getHop() > 0 && message.getHop() > this->getHop())
        return false;

    //compare targets
    if(!this->getTarget().isBroadcast()) {
        if(this->getTarget().getVendorId() != "*" && message.getTarget().getVendorId() != this->getTarget().getVendorId())
            return false;
        if(this->getTarget().getDeviceId() != "*" && message.getTarget().getDeviceId() != this->getTarget().getDeviceId())
            return false;
        if(this->getTarget().getInstanceId() != "*" && message.getTarget().getInstanceId() != this->getTarget().getInstanceId())
            return false;
    }

    //compare sources
    if(!this->getSource().isBroadcast()) {
        if(this->getSource().getVendorId() != "*" && message.getSource().getVendorId() != this->getSource().getVendorId())
            return false;
        if(this->getSource().getDeviceId() != "*" && message.getSource().getDeviceId() != this->getSource().getDeviceId())
            return false;
        if(this->getSource().getInstanceId() != "*" && message.getSource().getInstanceId() != this->getSource().getInstanceId())
            return false;
    }

    //compare xPL schemas
    if(this->getMessageSchema() != "*" && !this->getMessageSchema().isEmpty()
            && this->getMessageSchema() != message.getMessageSchema())
        return false;
    if(this->getMessageClass() != "*" && !this->getMessageClass().isEmpty()
            && this->getMessageClass() != message.getMessageClass())
        return false;

    return true;
}
