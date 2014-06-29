#ifndef QXPLCONFIGVALUE_H
#define QXPLCONFIGVALUE_H

#include <QString>

class QxPLConfigValue
{
public:
    typedef enum {
        CONFIG = 0x00,  //this type of value is required and can only be configured
                        //at device startup and it cannot be altered later
        RECONF = 0x01,  //this type of value is required and can be re-configured at any time
        OPTION = 0x02   //optional field that may be omitted when responding
    } QxPLValueType;

private:
    QString key;
    QString value;
    QxPLValueType type;

public:
    QxPLConfigValue();
    QxPLConfigValue(const QString& key, const QString& value);
    QxPLConfigValue(const QxPLConfigValue& other);
    QxPLConfigValue& operator=(const QxPLConfigValue& other);
    bool equals(const QxPLConfigValue& other) const;
    const QString& getKey() const;
    void setKey(const QString& key);
    const QString& getValue() const;
    void setValue(const QString& value);
    QxPLValueType getType() const;
    void setType(QxPLValueType type);
};

bool operator==(const QxPLConfigValue& a, const QxPLConfigValue& b);

#endif // QXPLCONFIGVALUE_H
