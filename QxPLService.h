#ifndef QXPLSERVICE_H
#define QXPLSERVICE_H

//#include <QThread>
#include <QUdpSocket>
#include <QList>
#include <QTimer>
#include <QStringList>
#include <QByteArray>
#include <QNetworkInterface>

#include "QxPLFilter.h"
#include "QxPLIdentifier.h"
#include "QxPLMessage.h"
#include "QxPLConfigValue.h"

/**
 * @brief The QxPLService class
 */
class QxPLService : public QObject
{
    Q_OBJECT
private:
    QxPLIdentifier identifier;
    QUdpSocket *socket;
    QList<QxPLFilter> m_filters;
    QList<QxPLConfigValue> m_configValueList;
    QString ipAddress;
    int port;
    static int const XPL_MINIMUM_PORT_AREA;
    static int const XPL_MAXIMUM_PORT_AREA;
    static int const XPL_DEFAULT_PORT;
    QTimer* heartbeatTimer;
    int heartbeatInterval;
    int discoveringHeartbeatInterval;
    bool hubDiscovered;
    bool fixedInstanceId;
    bool listening;

    void processDatagram(const QByteArray& datagram);
    void processMessage(const QxPLMessage& message);
    QString retrieveLocalIpAddress() const;
    bool applyFilters(const QxPLMessage& message) const;

public:
    QxPLService(const QxPLIdentifier& identifier, QObject* parent = 0);
    QxPLService(const QString& identifier, QObject* parent = 0);
    virtual ~QxPLService();
    const QxPLIdentifier& getIdentifier() const;
    void setIdentifier(const QxPLIdentifier& identifier);
    const QString& getIpAddress() const;
    int getPort() const;
    int getHeartbeatInterval() const;
    bool isHubDicovered() const;
    bool isFixedInstanceId() const;
    QList<QxPLFilter>& filters();
    const QList<QxPLFilter>& filters() const;
    bool isListening() const;
    QList<QxPLConfigValue>& configValueList();
    const QList<QxPLConfigValue>& configValueList() const;
    void addConfigValue(const QString& key, const QString& defaultValue = "");
    void removeConfigValue(const QString& key);
    const QString& configValue(const QString& key, const QString& defaultValue = "") const;

private slots:
    void readPendingDatagrams();
    void heartbeatTimeout();

protected slots:
    virtual void xPLMessageProcess(QxPLMessage message);

public slots:
    void listen();
    void close();
    void sendMessage(QxPLMessage message);
    void sendHeartbeat();
    void setFixedInstanceId(bool b);
    void setHeartbeatInterval(int interval);
    void onConfigurationMessageReceived(const QxPLMessage &message);

signals:
    void messageReceived(QxPLMessage message);
};

#endif // QXPLSERVICE_H
