#include "QxPLUtils.h"

namespace xPL4Qt {

    QStringList function_arg_list(const QString &function)
    {
        if(!function.contains("(") || !function.contains(")")) {
            return QStringList();
        }
        QString str_list = function.right(function.length() - function.indexOf("(") - 1);
        str_list = str_list.left(str_list.length() - 1);
        return str_list.split(",");
    }

    QString function_name(const QString &function)
    {
        if(!function.contains("(") || !function.contains(")")) {
            return "";
        }
        return function.left(function.indexOf("("));
    }

}
