#include "QxPLMessage.h"
//----------------------------------------------------------------
QxPLMessage::QxPLMessage(int hop)
{
    this->type = NONE;
    this->messageClass = "";
    this->messageSchema = "";
    this->hop = hop;
}
//----------------------------------------------------------------
QxPLMessage::QxPLMessage(QxPLMessageType type, int hop)
{
    this->type = type;
    this->hop = hop;
    this->messageClass = "";
    this->messageSchema = "";
}
//----------------------------------------------------------------
QxPLMessage::QxPLMessage(QxPLMessageType type, const QString &messageSchema, const QString &messageClass, int hop)
{
    this->type = type;
    this->hop = hop;
    this->messageClass = messageClass;
    this->messageSchema = messageSchema;
}
//----------------------------------------------------------------
QxPLMessage::QxPLMessage(QxPLMessageType type, const QxPLIdentifier &source, const QxPLIdentifier &target, int hop)
{
    this->type = type;
    this->hop = hop;
    this->source = source;
    this->target = target;
}
//----------------------------------------------------------------
QxPLMessage::QxPLMessage(QxPLMessageType type, const QxPLIdentifier &source, const QxPLIdentifier &target,
                         const QString &messageSchema, const QString &messageClass, int hop)
{
    this->type = type;
    this->hop = hop;
    this->messageClass = messageClass;
    this->messageSchema = messageSchema;
    this->source = source;
    this->target = target;
}
//----------------------------------------------------------------
QxPLMessage::QxPLMessage(const QxPLMessage &other)
{
    this->type = other.type;
    this->hop = other.hop;
    this->messageClass = other.messageClass;
    this->messageSchema = other.messageSchema;
    this->source = other.source;
    this->target = other.target;
    //copy nvlist
    for(int i = 0; i < other.nvlist.count(); i++) {
        this->nvlist.append(other.nvlist.at(i));
    }
}
//----------------------------------------------------------------
QxPLMessage& QxPLMessage::operator=(const QxPLMessage& other)
{
    this->type = other.type;
    this->hop = other.hop;
    this->messageClass = other.messageClass;
    this->messageSchema = other.messageSchema;
    this->source = other.source;
    this->target = other.target;
    //duplicate nvlist
    this->nvlist.clear();
    for(int i = 0; i < other.nvlist.count(); i++) {
        this->nvlist.append(other.nvlist.at(i));
    }
    return *this;
}
//----------------------------------------------------------------
QString QxPLMessage::toString() const
{
    QString rslt = "";
    switch(type) {
    case COMMAND:
        rslt += "xpl-cmnd\n";
        break;
    case STATUS:
        rslt += "xpl-stat\n";
        break;
    case TRIGGER:
        rslt += "xpl-trig\n";
        break;
    default:
        return "";
    }
    rslt += "{\n";
    rslt += "hop="+QString::number(hop)+"\n";
    rslt += "source="+source.toString()+"\n";
    rslt += "target="+target.toString()+"\n";
    rslt += "}\n";
    rslt += messageSchema+"."+messageClass+"\n";
    rslt += "{\n";
    for(int i = 0; i < nvlist.count(); i++) {
        rslt += nvlist.at(i).toString()+"\n";
    }
    rslt += "}\n";
    return rslt;
}
//----------------------------------------------------------------
bool QxPLMessage::contains(const QString &name) const
{
    for(int i = 0; i < nvlist.count(); i++) {
        if(name == nvlist.at(i).getName())
            return true;
    }
    return false;
}
//----------------------------------------------------------------
int QxPLMessage::count(const QString &name) const
{
    if(name.isEmpty())
        return nvlist.count();
    int rslt = 0;
    for(int i = 0; i < nvlist.count(); i++) {
        if(name == nvlist.at(i).getName())
            rslt++;
    }
    return rslt;
}
//----------------------------------------------------------------
QString QxPLMessage::getNamedValue(const QString &name, int occurrence) const
{
    if(occurrence < 0)
        return "";
    int iOccurrence = 0;
    for(int i = 0; i < nvlist.count(); i++) {
        if(name == nvlist.at(i).getName()) {
            if(occurrence == iOccurrence) {
                return nvlist.at(i).getValue();
            }
            iOccurrence++;
        }
    }
    return "";
}
//----------------------------------------------------------------
void QxPLMessage::setNamedValue(const QString &name, const QString &value, int occurence)
{
    if(occurence < 0)
        return;
    int iOccurrence = 0;
    for(int i = 0; i < nvlist.count(); i++) {
        if(nvlist.at(i).getName() == name) {
            if(iOccurrence == occurence) {
                nvlist[i].setValue(value);
                return;
            }
            iOccurrence++;
        }
    }
    nvlist.append(QxPLNameValuePair(name, value));
}
//----------------------------------------------------------------
void QxPLMessage::remove(const QString &name, int occurence)
{
    if(name.isEmpty())
        return;
    if(occurence < 0)
        return;
    int iOccurrence = 0;
    for(int i = 0; i < nvlist.count(); i++) {
        if(name == nvlist.at(i).getName()) {
            if(iOccurrence == 0) {
                nvlist.removeAt(i);
                return;
            }
            iOccurrence++;
        }
    }
}
//----------------------------------------------------------------
void QxPLMessage::removeAll(const QString &name)
{
    if(name.isEmpty()) {
        nvlist.clear();
    }
    else {
        for(int i = 0; i < nvlist.count(); i++) {
            if(name == nvlist.at(i).getName())
                nvlist.removeAt(i);
        }
    }
}
//----------------------------------------------------------------
void QxPLMessage::append(const QString &name, const QString &value)
{
    QxPLNameValuePair nvpair(name, value);
    nvlist.append(nvpair);
}
//----------------------------------------------------------------
