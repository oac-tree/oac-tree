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

}  // namespace Utils

}  // namespace Model

}  // namespace Sequencer
