#ifndef QXPLFILTER_H
#define QXPLFILTER_H

#include "QxPLMessage.h"

class QxPLFilter : public QxPLMessage
{
private:

public:
    QxPLFilter();
    bool match(const QxPLMessage& message) const;

};

#endif // QXPLFILTER_H
