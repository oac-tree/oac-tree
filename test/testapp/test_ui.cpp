
#include "test_ui.h"

#include <sup/sequencer/instruction.h>

#include <iostream>

using namespace sup::sequencer;

TestUI::TestUI() = default;

TestUI::~TestUI() = default;

void TestUI::UpdateInstructionStatus(const Instruction* instruction)
{
  auto instruction_type = instruction->GetType();
  auto status = instruction->GetStatus();

  std::cout << "Instruction: " << instruction_type << ": ";
  std::cout << StatusToString(status) << std::endl;
}
