#ifndef QXPLUTILS_H
#define QXPLUTILS_H

#include <QStringList>

namespace xPL4Qt {
    QStringList function_arg_list(const QString& exp);
    QString function_name(const QString& exp);
}

#endif // QXPLUTILS_H
