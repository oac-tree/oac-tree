#ifndef ITEMTYPES_H
#define ITEMTYPES_H

#include <string>

/**
 * @brief Definition of the available Item type names.
 */
namespace Sequencer {

namespace Model {

using ItemType = std::string;

const ItemType SequenceType = "Sequence";
const ItemType VariableType = "Variable";
const ItemType StringType = "String";
const ItemType Int32Type = "Int32";

}  // namespace Model

}  // namespace Sequencer

#endif // ITEMTYPES_H
