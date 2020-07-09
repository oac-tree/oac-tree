#include "TestModel.h"

#include "model/ProjectItem.h"
#include "model/SequenceItem.h"
#include "model/InstructionItem.h"
#include "model/VariableItem.h"

namespace Sequencer {

namespace TestModel {

Model::Item * CreateTestModel()
{
    auto root_item = new Model::ProjectItem();
    auto var_item1 = new Model::VariableItem();
    auto var_item2 = new Model::VariableItem();
    auto var_item3 = new Model::VariableItem();
    auto var_item4 = new Model::VariableItem();
    root_item->InsertItem(var_item1, Model::VariableType);
    root_item->InsertItem(var_item2, Model::VariableType);
    root_item->InsertItem(var_item3, Model::VariableType);
    root_item->InsertItem(var_item4, Model::VariableType);
    auto sequence_item = new Model::SequenceItem();
    auto instruction_item1 = new Model::InstructionItem();
    auto instruction_item2 = new Model::InstructionItem();
    auto instruction_item3 = new Model::InstructionItem();
    sequence_item->InsertItem(instruction_item1, Model::InstructionType);
    sequence_item->InsertItem(instruction_item2, Model::InstructionType);
    sequence_item->InsertItem(instruction_item3, Model::InstructionType);
    root_item->InsertItem(sequence_item, Model::SequenceType);
    return root_item;
}

}  // namespace TestModel

}  // namespace Sequencer
