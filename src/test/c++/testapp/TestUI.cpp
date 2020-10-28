#include "TestUI.h"

#include <iostream>


TestUI::TestUI() = default;

TestUI::~TestUI() = default;

void TestUI::UpdateInstructionStatus(const Instruction * instruction)
{
    auto instruction_type = instruction->GetType();
    auto status = instruction->GetStatus();

    std::cout << "Instruction: " << instruction_type << ": ";
    std::cout << StatusToString(status) << std::endl;
}

void TestUI::StartSingleStep()
{
    std::cout << "Start single execution step" << std::endl;
}

void TestUI::EndSingleStep()
{
    std::cout << "End single execution step" << std::endl;
}
