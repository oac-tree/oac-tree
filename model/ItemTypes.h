#ifndef ITEMTYPES_H
#define ITEMTYPES_H

#include <string>

namespace Sequencer {

namespace Model {

using ItemType = std::string;

const ItemType StringType = "String";
const ItemType Int32Type = "Int32";

}  // namespace Model

}  // namespace Sequencer

#endif // ITEMTYPES_H
