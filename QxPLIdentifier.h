/*
    This file is part of Zigbus System.

    Zigbus System is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Zigbus System is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Zigbus System.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QXPLIDENTIFIER_H
#define QXPLIDENTIFIER_H

#include <QString>

/**
 * @author jhx
 * @brief The QxPLIdentifier class represents the xPL identifier of a xPL device.
 * Any xPL device has a unique identifier on its xPL network. It is a combination of three litteral strings:<br>
 * <ul>
 *  <li>the vendor id</li>
 *  <li>the device id</li>
 *  <li>the instance id</li>
 * </ul><br>
 *
 * In a xPL message, you will retrieve the identifier as the following representation :
 *  <vendor id>-<device id>.<instance id>
 * A call to toString() will generate a character string with the above format.
 * Calling the QxPLIdentifier() constructor with a litteral string formated as above will
 * generate a valid QxPLIdentifier object.
 */
class QxPLIdentifier
{
private:
    /**
     * @brief vendorId
     */
    QString vendorId;

    /**
     * @brief deviceId
     */
    QString deviceId;

    /**
     * @brief instanceId
     */
    QString instanceId;

public:
    /**
     * @brief QxPLIdentifier
     */
    QxPLIdentifier();

    /**
     * @brief QxPLIdentifier
     * @param vendorId
     * @param deviceId
     * @param instanceId
     */
    QxPLIdentifier(const QString& vendorId, const QString& deviceId, const QString& instanceId);

    /**
     * @brief QxPLIdentifier
     * @param other
     */
    QxPLIdentifier(const QxPLIdentifier& other);

    /**
     * @brief QxPLIdentifier
     * @param str
     */
    QxPLIdentifier(const QString& str);

    /**
     * @brief operator =
     * @param other
     * @return
     */
    QxPLIdentifier& operator=(const QxPLIdentifier& other);

    /**
     * @brief equals
     * @param other
     * @return
     */
    bool equals(const QxPLIdentifier& other) const;

    /**
     * @brief isValid evaluates if the identifier is valid. A valid identifier is
     * an identifier which has a non empty vendorId and a non empty deviceId.
     * @return Returns true if the identifier is valid. Otherwise, returns false
     */
    bool isValid() const;

    /**
     * @brief toString
     * @return
     */
    QString toString() const;

    /**
     * @brief isBroadcast
     * @return
     */
    bool isBroadcast() const;

    /**
     * @brief broadcast
     * @return
     */
    static QxPLIdentifier broadcast();

    /**
     * @brief getVendorId
     * @return
     */
    const QString& getVendorId() const { return vendorId; }

    /**
     * @brief setVendorId
     * @param vendorId
     */
    void setVendorId(const QString& vendorId) { this->vendorId = vendorId; }

    /**
     * @brief getDeviceId
     * @return
     */
    const QString& getDeviceId() const { return deviceId; }

    /**
     * @brief setDeviceID
     * @param deviceId
     */
    void setDeviceID(const QString& deviceId) { this->deviceId = deviceId; }

    /**
     * @brief getInstanceId
     * @return
     */
    const QString& getInstanceId() const { return instanceId; }

    /**
     * @brief setInstanceId
     * @param instanceId
     */
    void setInstanceId(const QString& instanceId) { this->instanceId = instanceId; }
};

bool operator==(const QxPLIdentifier& a, const QxPLIdentifier& b);

bool operator!=(const QxPLIdentifier& a, const QxPLIdentifier& b);

#endif // QXPLIDENTIFIER_H
