#include "TestModel.h"

#include "model/ProjectItem.h"
#include "model/PrimitiveItems.h"

namespace Sequencer {

namespace TestModel {

std::vector<std::string> cities = { "Paris", "New York", "Sao Paulo" };

Model::Item * CreateTestModel()
{
    auto root_item = new Model::ProjectItem();
    for (auto& city : cities)
    {
        auto item = new Model::StringItem(city);
        root_item->AddChild(item);
    }
    return root_item;
}

}  // namespace TestModel

}  // namespace Sequencer
