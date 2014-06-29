#ifndef QXPLMESSAGE_H
#define QXPLMESSAGE_H

#include <QList>
#include <QDebug>

#include "QxPLNameValuePair.h"
#include "QxPLIdentifier.h"


/**
 * @brief The QxPLMessage class
 */
class QxPLMessage
{
public:

    /**
     * @brief The XPL_MESSAGE_TYPE enum
     */
    enum XPL_MESSAGE_TYPE {
        NONE,
        COMMAND,
        TRIGGER,
        STATUS,
        ANY
    };

    /**
     * @brief QxPLMessageType
     */
    typedef enum XPL_MESSAGE_TYPE QxPLMessageType;
private:
    /**
     * @brief type
     */
    QxPLMessageType type;

    /**
     * @brief hop
     */
    int hop;

    /**
     * @brief source
     */
    QxPLIdentifier source;

    /**
     * @brief target
     */
    QxPLIdentifier target;

    /**
     * @brief messageClass
     */
    QString messageClass;

    /**
     * @brief messageSchemas
     */
    QString messageSchema;

    /**
     * @brief nvlist
     */
    QList<QxPLNameValuePair> nvlist;

public:
    /**
     * @brief QxPLMessage
     */
    QxPLMessage(int hop = 1);

    /**
     * @brief QxPLMessage
     * @param type
     * @param hop
     */
    QxPLMessage(QxPLMessageType type, int hop = 1);

    /**
     * @brief QxPLMessage
     * @param type
     * @param messageSchema
     * @param messageClass
     * @param hop
     */
    QxPLMessage(QxPLMessageType type, const QString& messageSchema, const QString& messageClass, int hop = 1);

    /**
     * @brief QxPLMessage
     * @param type
     * @param source
     * @param target
     * @param hop
     */
    QxPLMessage(QxPLMessageType type, const QxPLIdentifier& source, const QxPLIdentifier& target, int hop = 1);

    /**
     * @brief QxPLMessage
     * @param type
     * @param source
     * @param target
     * @param messageSchema
     * @param messageClass
     * @param hop
     */
    QxPLMessage(QxPLMessageType type, const QxPLIdentifier& source, const QxPLIdentifier& target,
                const QString& messageSchema, const QString& messageClass, int hop = 1);

    /**
     * @brief QxPLMessage
     * @param other
     */
    QxPLMessage(const QxPLMessage& other);

    /**
     * @brief operator =
     * @param other
     * @return
     */
    QxPLMessage& operator=(const QxPLMessage& other);

    /**
     * @brief toString
     * @return
     */
    QString toString() const;

    /**
     * @brief contains
     * @param name
     * @return
     */
    bool contains(const QString& name) const;

    /**
     * @brief count
     * @param name
     * @return
     */
    int count(const QString& name = "") const;

    /**
     * @brief getNamedValue
     * @param name
     * @param occurrence
     * @return
     */
    QString getNamedValue(const QString& name, int occurrence = 0) const;

    /**
     * @brief setNamedValue
     * @param name
     * @param value
     * @param occurence
     */
    void setNamedValue(const QString& name, const QString& value, int occurence = 0);

    /**
     * @brief append
     * @param name
     * @param value
     */
    void append(const QString& name, const QString& value);

    /**
     * @brief removeAll
     * @param name
     */
    void removeAll(const QString& name = "");

    /**
     * @brief remove
     * @param name
     * @param occurence
     */
    void remove(const QString& name, int occurence = 0);

    /**
     * @brief isBroadcast
     * @return
     */
    bool isBroadcast() const { return target.isBroadcast(); }

    /**
     * @brief getType
     * @return
     */
    QxPLMessageType getType() const { return type; }

    /**
     * @brief setType
     * @param type
     */
    void setType(QxPLMessageType type) { this->type = type; }

    /**
     * @brief getHop
     * @return
     */
    int getHop() const { return hop; }

    /**
     * @brief setHop
     * @param hop
     */
    void setHop(int hop) { this->hop = hop; }

    /**
     * @brief getSource
     * @return
     */
    const QxPLIdentifier& getSource() const { return source; }

    /**
     * @brief setSource
     * @param source
     */
    void setSource(const QxPLIdentifier& source) { this->source = source; }

    /**
     * @brief getTarget
     * @return
     */
    const QxPLIdentifier& getTarget() const { return target; }

    /**
     * @brief setTarget
     * @param target
     */
    void setTarget(const QxPLIdentifier& target) { this->target = target; }

    /**
     * @brief getMessageSchema
     * @return
     */
    const QString& getMessageSchema() const { return messageSchema; }

    /**
     * @brief setMessageSchema
     * @param messageSchema
     */
    void setMessageSchema(const QString& messageSchema) { this->messageSchema = messageSchema; }

    /**
     * @brief getMessageClass
     * @return
     */
    const QString& getMessageClass() const { return messageClass; }

    /**
     * @brief setMessageClass
     * @param messageClass
     */
    void setMessageClass(const QString& messageClass) { this->messageClass = messageClass; }
};

Q_DECLARE_METATYPE(QxPLMessage)

#endif // QXPLMESSAGE_H
