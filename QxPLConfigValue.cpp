#include "QxPLConfigValue.h"

/**
   @brief QxPLConfigValue::QxPLConfigValue
 */
QxPLConfigValue::QxPLConfigValue()
{
    type = CONFIG;
}

/**
   @brief QxPLConfigValue::QxPLConfigValue
   @param key
   @param value
 */
QxPLConfigValue::QxPLConfigValue(const QString &key, const QString &value)
{
    this->key = key;
    this->value = value;
    type = CONFIG;
}

/**
   @brief QxPLConfigValue::QxPLConfigValue
   @param other
 */
QxPLConfigValue::QxPLConfigValue(const QxPLConfigValue &other)
{
    this->key = other.key;
    this->value = other.value;
    this->type = other.type;
}

/**
   @brief QxPLConfigValue::operator =
   @param other
   @return
 */
QxPLConfigValue& QxPLConfigValue::operator =(const QxPLConfigValue& other)
{
    this->key = other.key;
    this->value = other.value;
    this->type = other.type;
    return *this;
}

/**
   @brief QxPLConfigValue::equals
   @param other
   @return
 */
bool QxPLConfigValue::equals(const QxPLConfigValue &other) const
{
    return (this->key == other.key && this->value == other.value
            && this->type == other.type);
}

/**
   @brief operator ==
   @param a
   @param b
   @return
 */
bool operator==(const QxPLConfigValue& a, const QxPLConfigValue& b)
{
    return a == b;
}

/**
   @brief QxPLConfigValue::getKey
   @return
 */
const QString& QxPLConfigValue::getKey() const
{
    return key;
}

/**
   @brief QxPLConfigValue::setKey
   @param key
 */
void QxPLConfigValue::setKey(const QString &key)
{
    this->key = key;
}

/**
   @brief QxPLConfigValue::getValue
   @return
 */
const QString& QxPLConfigValue::getValue() const
{
    return value;
}

/**
   @brief QxPLConfigValue::setValue
   @param value
 */
void QxPLConfigValue::setValue(const QString &value)
{
    this->value = value;
}

QxPLConfigValue::QxPLValueType QxPLConfigValue::getType() const
{
    return this->type;
}

void QxPLConfigValue::setType(QxPLValueType type)
{
    this->type = type;
}
