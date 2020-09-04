#include "TestUI.h"

#include <iostream>


TestUI::TestUI() = default;

TestUI::~TestUI() = default;

void TestUI::UpdateInstructionStatus(const Instruction * instruction)
{
    auto status = instruction->GetStatus();

    std::cout << "status: " << StatusToString(status) << std::endl;
}
