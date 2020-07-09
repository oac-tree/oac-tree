#include "UserTypes.h"

namespace Sequencer {

namespace Model {

namespace Utils {

bool CompatibleVariantTypes(const QVariant & lval, const QVariant & rval)
{
    if (!lval.isValid() || !rval.isValid())
    {
        return true;
    }
    return lval.userType() == rval.userType();
}

bool Equals(const QVariant &lval, const QVariant &rval)
{
    if (lval.userType() != rval.userType())
    {
        return false;
    }
    return lval == rval;
}

QVariant ToQtVariant(const QVariant & custom)
{
    // converts variant based on std::string to variant based on QString
    if (custom.typeName() == std::string("std::string")) {
        return QVariant(QString::fromStdString(custom.value<std::string>()));
    }
    // in other cases returns unchanged variant
    return custom;
}

QVariant ToCustomVariant(const QVariant & standard)
{
    const QString string_name = "QString";

    // converts variant based on QString to variant based on std::string
    if (standard.typeName() == string_name)
        return QVariant::fromValue(standard.toString().toStdString());

    // in other cases returns unchanged variant
    return standard;
}

}  // namespace Utils

}  // namespace Model

}  // namespace Sequencer
