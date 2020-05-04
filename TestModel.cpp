#include "TestModel.h"

#include "model/ProjectItem.h"
#include "model/SequenceItem.h"
#include "model/InstructionItem.h"
#include "model/PrimitiveItems.h"

namespace Sequencer {

namespace TestModel {

std::vector<std::string> cities = { "Paris", "New York", "Sao Paulo" };

Model::Item * CreateTestModel()
{
//    auto root_item = new Model::ProjectItem();
    auto sequence_item = new Model::SequenceItem();
    auto instruction_item1 = new Model::InstructionItem();
    auto instruction_item2 = new Model::InstructionItem();
    auto instruction_item3 = new Model::InstructionItem();
    sequence_item->InsertItem(instruction_item1, Model::InstructionType);
    sequence_item->InsertItem(instruction_item2, Model::InstructionType);
    sequence_item->InsertItem(instruction_item3, Model::InstructionType);
//    root_item->InsertItem(sequence_item, Model::SequenceType);
    return sequence_item;
}

}  // namespace TestModel

}  // namespace Sequencer
