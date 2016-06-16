#ifndef DENUMCONSTANTS_H
#define DENUMCONSTANTS_H

#include <QMap>
#include <QString>

namespace D {

struct EnumConstants
{
    EnumConstants();

    typedef QMap<QString, QString> StringMap;
    StringMap m_enumCppToD;
};

extern EnumConstants enumConstants;
QString enumCpptoD(QString enumVal);

}

#endif // DENUMCONSTANTS_H
