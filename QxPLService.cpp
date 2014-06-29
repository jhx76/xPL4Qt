#include "QxPLService.h"

int const QxPLService::XPL_MINIMUM_PORT_AREA = 49152;
int const QxPLService::XPL_MAXIMUM_PORT_AREA = 65535;
int const QxPLService::XPL_DEFAULT_PORT = 3865;

/**
   @brief QxPLService::QxPLService
   @param identifier
   @param parent
 */
QxPLService::QxPLService(const QxPLIdentifier& identifier, QObject* parent) : QObject(parent) {
    hubDiscovered = false;
    fixedInstanceId = true;
    listening = false;
    discoveringHeartbeatInterval = 3*1000; //default is 3sec
    heartbeatInterval = 5*1000*60; //default is 5minutes
    ipAddress = "";
    port = -1;
    socket = NULL;
    heartbeatTimer = NULL;
    this->identifier = identifier;
    connect(this, SIGNAL(messageReceived(QxPLMessage)), this, SLOT(xPLMessageProcess(QxPLMessage)));
}

/**
   @brief QxPLService::QxPLService
   @param identifier
   @param parent
 */
QxPLService::QxPLService(const QString &identifier, QObject *parent) : QObject(parent) {
    hubDiscovered = false;
    fixedInstanceId = true;
    listening = false;
    discoveringHeartbeatInterval = 3*1000; //default is 3sec
    heartbeatInterval = 5*1000*60; //default is 5minutes
    ipAddress = "";
    port = -1;
    socket = NULL;
    heartbeatTimer = NULL;
    this->identifier = QxPLIdentifier(identifier);
    connect(this, SIGNAL(messageReceived(QxPLMessage)), this, SLOT(xPLMessageProcess(QxPLMessage)));
}

/**
   @brief QxPLService::~QxPLService
 */
QxPLService::~QxPLService()
{
    close();
}

/**
   @brief QxPLService::listen
 */
void QxPLService::listen() {
    qDebug() << "[xpl] initialize network access...";
    socket = new QUdpSocket();
    ipAddress = retrieveLocalIpAddress();
    if(ipAddress.isEmpty()) {
        qDebug() << "No network interface found on the host machine";
        return;
    }
    qDebug() << "[xPL] Host ip address found : " << ipAddress;
    port = XPL_MINIMUM_PORT_AREA;
    while(!socket->bind(port, QUdpSocket::ShareAddress)) {
        port++;
        if(port > XPL_MAXIMUM_PORT_AREA) {
            qDebug() << "[xPL] No port available";
            return;
        }
    }
    qDebug() << "[xpl] Host port found to map on the hub : " << port;
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    listening = true;

    /*
     * Send a heartbeat every 10 seconds until the application receives its
     * own echo. Then the hub will be discovered and we will set a more efficient
     * heartbeat interval.
     */
    heartbeatTimer = new QTimer(this);
    heartbeatTimer->setInterval(discoveringHeartbeatInterval);
    qDebug() << "[xpl] Initializing heartbeat every "
             << QString::number(discoveringHeartbeatInterval)
             << "msec until the hub discovery";
    QObject::connect(heartbeatTimer, SIGNAL(timeout()), this, SLOT(heartbeatTimeout()));
    heartbeatTimer->start();
}

void QxPLService::close()
{
    qDebug() << "[xpl] closing communication...";
    if(socket) {
        if(socket->isOpen())
            socket->close();
        delete socket;
    }
    listening = false;
    qDebug() << "[xpl] communication closed !";
}

QString QxPLService::retrieveLocalIpAddress() const
{
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            return address.toString();
    }
    return "";
}

void QxPLService::heartbeatTimeout()
{
    heartbeatTimer->stop();
    //for cases where interval has changed during the execution
    heartbeatTimer->setInterval(this->hubDiscovered ? this->heartbeatInterval
                                                    : this->discoveringHeartbeatInterval);
    sendHeartbeat();
    heartbeatTimer->start();
}

void QxPLService::readPendingDatagrams()
{
    while(socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        processDatagram(datagram);
    }
}

void QxPLService::processDatagram(const QByteArray& datagram)
{
    QString strDatagram = datagram;
    QStringList xplPacket = strDatagram.trimmed().split("\n");
    int hop = 0;
    QxPLIdentifier source;
    QxPLIdentifier target;
    bool conversionOK = false;
    QxPLMessage::QxPLMessageType messageType;
    QString messageSchema = "";
    QString messageClass = "";

    QString strType = xplPacket.at(0);
    if(strType == "xpl-cmnd") {
        messageType = QxPLMessage::COMMAND;
    }
    else if(strType == "xpl-stat") {
        messageType = QxPLMessage::STATUS;
    }
    else if(strType == "xpl-trig") {
        messageType = QxPLMessage::TRIGGER;
    }
    else {
        qDebug() << "[SKIP] Bad syntax: " << strType << " is not a valid message type";
        return;
    }
    if(xplPacket.at(1) != "{")  {
        qDebug() << "[SKIP] Bad syntax: missing left bracket after type definition";
        return;
    }

    int i;
    for(i = 2; xplPacket.at(i) != "}" && i < xplPacket.count(); i++) {
        QString name = xplPacket.at(i).mid(0, xplPacket.at(i).indexOf("="));
        QString value = xplPacket.at(i).mid(xplPacket.at(i).indexOf("=")+1);
        if(name == "hop") {
            hop = value.toInt(&conversionOK);
            if(!conversionOK) {
                qDebug() << "[SKIP] Cannot convert " << value << " into integer";
                return;
            }
            if(hop < 1) {
                qDebug() << "[SKIP] Bad syntax: cant accept a message with a null hop count";
                return;
            }
        }
        else if(name == "source") {
            if(!value.contains("-") && !value.contains(".")) {
                qDebug() << "[SKIP] Bad syntax: wrong source identifier";
                return;
            }
            QString vendorId = value.mid(0, value.indexOf("-"));
            QString deviceId = value.mid(value.indexOf("-")+1);
            deviceId = deviceId.left(deviceId.indexOf("."));
            QString instanceId = value.mid(value.indexOf(".")+1);
            source.setVendorId(vendorId);
            source.setDeviceID(deviceId);
            source.setInstanceId(instanceId);
        }
        else if(name == "target") {
            if(value == "*")
                target = QxPLIdentifier::broadcast();
            else {
                if(!value.contains("-") && !value.contains(".")) {
                    qDebug() << "[SKIP] Bad syntax: wrong target identifier";
                    return;
                }
                QString vendorId = value.mid(0, value.indexOf("-"));
                QString deviceId = value.mid(value.indexOf("-")+1);
                deviceId = deviceId.left(deviceId.indexOf("."));
                QString instanceId = value.mid(value.indexOf(".")+1);
                target.setVendorId(vendorId);
                target.setDeviceID(deviceId);
                target.setInstanceId(instanceId);
            }
        }
    }
    i++; // jump over the bracket
    messageSchema = xplPacket.at(i).mid(0, xplPacket.at(i).indexOf("."));
    messageClass = xplPacket.at(i).mid(xplPacket.at(i).indexOf(".")+1);
    i++; //jump over the bracket
    if(xplPacket.at(i) != "{") {
        qDebug() << "[SKIP] Bad syntax: missing left bracket after message schema definition";
        return;
    }
    QxPLMessage message(messageType, source, target, messageSchema, messageClass, hop);

    i++;
    for(; i < xplPacket.count() && xplPacket.at(i) != "}"; i++) {
        QString name = xplPacket.at(i).mid(0, xplPacket.at(i).indexOf("="));
        QString value = xplPacket.at(i).mid(xplPacket.at(i).indexOf("=")+1);
        message.append(name, value);
    }
    processMessage(message);
}

void QxPLService::processMessage(const QxPLMessage &message)
{
    /* if the message is a heartbeat and is coming from this xPL service, the hub is discovered ! */
    if(!hubDiscovered && message.getSource() == getIdentifier()
            && message.getMessageSchema() == "hbeat" && message.getMessageClass() == "app"
            && message.getNamedValue("remote-ip") == ipAddress)
    {
        hubDiscovered = true;
        qDebug() << "[xpl] hub discovered !";
        qDebug() << "[xpl] heartbeat interval is now set to " << heartbeatInterval;
        qDebug() << "[xpl] communication is now available !";
    }

    if(applyFilters(message)) {
        emit messageReceived(message);
    }
}

bool QxPLService::applyFilters(const QxPLMessage& message) const
{
    if(m_filters.isEmpty())
        return true;
    for(int i = 0; i < m_filters.count(); i++) {
        if(m_filters.at(i).match(message))
            return true;
    }
    return false;
}

/**
   @brief QxPLService::sendMessage
   @param message
 */
void QxPLService::sendMessage(QxPLMessage message)
{
    if(this->isListening()) {
        QByteArray datagram = message.toString().toLocal8Bit();
        /*int rslt = */socket->writeDatagram(datagram.data(), datagram.size(),
                                         QHostAddress::Broadcast, XPL_DEFAULT_PORT);
        //qDebug() << "message sent (" << rslt << ")";
    }
}

/**
   @brief QxPLService::sendHeartbeat
 */
void QxPLService::sendHeartbeat()
{
    QxPLMessage message;
    message.setType(QxPLMessage::STATUS);
    message.setHop(1);
    message.setSource(this->getIdentifier());
    message.setTarget(QxPLIdentifier::broadcast());
    message.setMessageSchema("hbeat");
    message.setMessageClass("app");
    message.append("interval", QString::number(heartbeatInterval / 1000 / 60));
    message.append("port", QString::number(port));
    message.append("remote-ip", this->ipAddress);
    sendMessage(message);
}

/**
   @brief QxPLService::getIdentifier
   @return
 */
const QxPLIdentifier& QxPLService::getIdentifier() const
{
    return identifier;
}

/**
   @brief QxPLService::setIdentifier
   @param identifier
 */
void QxPLService::setIdentifier(const QxPLIdentifier& identifier)
{
    this->identifier = identifier;
}

/**
   @brief QxPLService::getIpAddress
   @return
 */
const QString& QxPLService::getIpAddress() const
{
    return ipAddress;
}

/**
   @brief QxPLService::getPort
   @return
 */
int QxPLService::getPort() const
{
    return port;
}

/**
   @brief QxPLService::getHeartbeatInterval
   @return
 */
int QxPLService::getHeartbeatInterval() const
{
    return heartbeatInterval;
}

/**
   @brief QxPLService::isHubDicovered
   @return
 */
bool QxPLService::isHubDicovered() const
{
    return hubDiscovered;
}

/**
   @brief QxPLService::isFixedInstanceId
   @return
 */
bool QxPLService::isFixedInstanceId() const
{
    return fixedInstanceId;
}

/**
   @brief QxPLService::filters
   @return
 */
QList<QxPLFilter>& QxPLService::filters()
{
    return m_filters;
}

/**
   @brief QxPLService::filters
   @return
 */
const QList<QxPLFilter>& QxPLService::filters() const
{
    return m_filters;
}

/**
   @brief QxPLService::isListening
   @return
 */
bool QxPLService::isListening() const
{
    return listening;
}

/**
   @brief QxPLService::setFixedInstanceId
   @param b
 */
void QxPLService::setFixedInstanceId(bool b)
{
    this->fixedInstanceId = b;
}

/**
   @brief QxPLService::setHeartbeatInterval
   @param interval
 */
void QxPLService::setHeartbeatInterval(int interval)
{
    this->heartbeatInterval = interval;
}

/**
   @brief QxPLService::xPLMessageProcess
 */
void QxPLService::xPLMessageProcess(QxPLMessage message)
{
    Q_UNUSED(message)
}

/**
   @brief QxPLService::xPL_config_response_cmd
   @param message
 */
void QxPLService::onConfigurationMessageReceived(const QxPLMessage &message)
{
    /*
        config.response
        {
        newconf=[new instance id]
        [interval=[value]]
        [group=[value]]
        [filter=[value]]
        [developer defined parameters...=[value]]
        ...
        }
      */
    bool conversionOK = false;
    int hbInterval = message.getNamedValue("interval").toInt(&conversionOK);
    if(!conversionOK) {
        qDebug() << "[SKIP] unreconized interval. Unable to convert into integer";
        return;
    }

    QString reconf = message.getNamedValue("newconf");
    if(reconf.isEmpty()) {
        qDebug() << "[SKIP] no newconf specified.";
        return;
    }

    QStringList groups;
    for(int i = 0; i < message.count("group"); i++) {
        groups << message.getNamedValue("group", i);
    }
    if(!groups.isEmpty()) {
        qDebug() << "[xPL] configuration value for groups is not processed. Not implemented yet !";
    }

    QStringList filters;
    for(int i = 0; i < message.count("filter"); i++) {
        filters << message.getNamedValue("filter", i);
    }
    if(!filters.isEmpty()) {
        qDebug() << "[xPL] configuration value for filters is not processed. Not implemented yet !";
    }

    /* Setup */
    for(int i = 0; i < m_configValueList.count(); i++) {
        QString key = m_configValueList.at(i).getKey();
        qDebug() << "[xPL] setup " << key << " parameter...";
        if(message.count() > 1) {

        }
        m_configValueList[i].setValue(message.getNamedValue(key));
    }

    qDebug() << "[xPL] setup new heartbeat interval to " << hbInterval;
    setHeartbeatInterval(hbInterval);

    identifier.setInstanceId(reconf);
    qDebug() << "[xPL] Configuration done. New instance id is " << identifier.getInstanceId();
}

/**
   @brief QxPLService::configValueList
   @return
 */
QList<QxPLConfigValue>& QxPLService::configValueList()
{
    return m_configValueList;
}

/**
   @brief QxPLService::configValueList
   @return
 */
const QList<QxPLConfigValue>& QxPLService::configValueList() const
{
    return m_configValueList;
}

/**
   @brief QxPLService::addConfigValue
   @param key
   @param defaultValue
 */
void QxPLService::addConfigValue(const QString &key, const QString &defaultValue)
{
    m_configValueList.append(QxPLConfigValue(key, defaultValue));
}

/**
   @brief QxPLService::removeConfigValue
   @param key
 */
void QxPLService::removeConfigValue(const QString &key)
{
    for(int i = 0; i < m_configValueList.count(); i++) {
        if(m_configValueList.at(i).getKey() == key) {
            m_configValueList.removeAt(i);
        }
    }
}

/**
   @brief QxPLService::configValue
   @param key
   @param defaultValue
   @return
 */
const QString& QxPLService::configValue(const QString &key, const QString &defaultValue) const
{
    for(int i = 0; i < m_configValueList.count(); i++) {
        if(m_configValueList.at(i).getKey() == key) {
            return m_configValueList.at(i).getValue();
        }
    }
    return defaultValue;
}

