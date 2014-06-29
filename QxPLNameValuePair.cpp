#include "QxPLNameValuePair.h"

QxPLNameValuePair::QxPLNameValuePair() {
    name = "";
    value = "";
}

QxPLNameValuePair::QxPLNameValuePair(const QString &name, const QString &value) {
    this->name = name;
    this->value = value;
}

QxPLNameValuePair::QxPLNameValuePair(const QxPLNameValuePair &other) {
    this->name = other.name;
    this->value = other.value;
}

QxPLNameValuePair& QxPLNameValuePair::operator=(const QxPLNameValuePair& other) {
    this->name = other.name;
    this->value = other.value;
    return *this;
}

bool QxPLNameValuePair::equals(const QxPLNameValuePair &other) const {
    return (this->name == other.name)
            && (this->value == other.value);
}

QString QxPLNameValuePair::toString() const {
    return name+"="+value;
}

bool operator==(const QxPLNameValuePair& a, const QxPLNameValuePair& b) {
    return a.equals(b);
}
