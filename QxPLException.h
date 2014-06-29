#ifndef QXPLEXCEPTION_H
#define QXPLEXCEPTION_H

#include <JLib/JException.h>

class QxPLException : public JException
{
public:
    QxPLException() throw();
    QxPLException(const QString& message, const JCodePosition& sourceLink) throw();
    QxPLException(const QxPLException& other) throw();
    virtual QString toString() const throw();
};

#endif // QXPLEXCEPTION_H
