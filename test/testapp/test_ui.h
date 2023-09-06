#ifndef SUP_SEQUENCER_TEST_UI_H_
#define SUP_SEQUENCER_TEST_UI_H_

#include <sup/sequencer/user_interface.h>

class TestUI : public sup::sequencer::DefaultUserInterface
{
public:
  TestUI();

  ~TestUI() override;

  void UpdateInstructionStatus(const sup::sequencer::Instruction* instruction) override;
};

#endif  // SUP_SEQUENCER_TEST_UI_H_
