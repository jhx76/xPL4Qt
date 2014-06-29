#ifndef QXPLNAMEVALUEPAIR_H
#define QXPLNAMEVALUEPAIR_H

#include <QString>

/**
 * @brief The QxPLNameValuePair class represents a named value with a QxPLMessage
 * @author jhx
 * @see QxPLMessage
 * @date 09/11/2013
 */
class QxPLNameValuePair
{
private:
    /**
     * @brief name represents the name of the named-value
     */
    QString name;

    /**
     * @brief value represents the value of the named-value
     */
    QString value;

public:
    /**
     * @brief QxPLNameValuePair Build an instance of the QxPLNameValuePair class.
     */
    QxPLNameValuePair();

    /**
     * @brief QxPLNameValuePair Build an instance of the QxPLNameValuePair class.
     * @param name The name to set
     * @param value The value to set
     */
    QxPLNameValuePair(const QString& name, const QString& value);

    /**
     * @brief QxPLNameValuePair Build an instance of the QxPLNameValuePair class.
     * @param other The instance to be copied
     */
    QxPLNameValuePair(const QxPLNameValuePair& other);

    /**
     * @brief operator =
     * @param other
     * @return
     */
    QxPLNameValuePair& operator=(const QxPLNameValuePair& other);

    /**
     * @brief equals Compare this instance to another one.
     * @param other
     * @return true if the @arg other is equivalent to this.
     */
    bool equals(const QxPLNameValuePair& other) const;

    /**
     * @brief toString
     * @return return a string that represents a named value within a xPL message.
     */
    QString toString() const;

    /**
     * @brief getName
     * @return The name
     */
    const QString& getName() const { return name; }

    /**
     * @brief setName
     * @param name The name to set
     */
    void setName(const QString& name) { this->name = name; }

    /**
     * @brief getValue
     * @return The value
     */
    const QString& getValue() const { return value; }

    /**
     * @brief setValue
     * @param value The value to set
     */
    void setValue(const QString& value) { this->value = value; }
};

/**
 * @brief operator ==
 * Compare to instance of QxPLNameValuePair class.
 * @param a
 * @param b
 * @return true if the two instances are equivalent
 */
bool operator==(const QxPLNameValuePair &a, const QxPLNameValuePair &b);

#endif // QXPLNAMEVALUEPAIR_H
