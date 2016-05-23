#include "denumconstants.h"

struct EnumConstantEntry
{
    const char *cppName;
    const char *dName;
};

static const EnumConstantEntry qenum_map[] = {
#define QT_ENUM_CONSTANT(cppName, dName) { #cppName, #dName },
#include "qenum_constants.h"

#undef QT_ENUM_CONSTANT
};

namespace D {

EnumConstants::EnumConstants()
{
    const EnumConstantEntry *enumEnd = qenum_map + sizeof(qenum_map)/sizeof(EnumConstantEntry);
    for (const EnumConstantEntry *it = qenum_map; it < enumEnd;  ++it) {
        const QString cppName = QLatin1String(it->cppName);
        const QString dName = QLatin1String(it->dName);
        m_enumCppToD.insert(cppName, dName);
    }
}

EnumConstants enumConstants;

}