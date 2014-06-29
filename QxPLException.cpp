#include "QxPLException.h"

QxPLException::QxPLException() throw()
    : JException()
{

}

QxPLException::QxPLException(const QString& message, const JCodePosition& sourceLink) throw()
    : JException(message, sourceLink)
{

}

QxPLException::QxPLException(const QxPLException& other) throw()
    : JException(other)
{

}

QString QxPLException::toString() const throw() {
    return getCodePosition().toString()+" [QxPLException] "+getMessage();
}
