#ifndef USERTYPES_H
#define USERTYPES_H

#include <QVariant>

namespace Sequencer {

namespace Model {

namespace Utils {

bool CompatibleVariantTypes(const QVariant & lval, const QVariant & rval);

bool Equals(const QVariant & lval, const QVariant & rval);

QVariant ToQtVariant(const QVariant & custom);

QVariant ToCustomVariant(const QVariant & standard);

}  // namespace Utils

}  // namespace Model

}  // namespace Sequencer

Q_DECLARE_METATYPE(std::string)

#endif // USERTYPES_H
