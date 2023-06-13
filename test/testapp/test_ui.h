#ifndef SUP_SEQUENCER_TEST_UI_H_
#define SUP_SEQUENCER_TEST_UI_H_

#include <sup/sequencer/user_interface.h>

class TestUI : public sup::sequencer::UserInterface
{
public:
  TestUI();

  ~TestUI() override;

private:
  void UpdateInstructionStatusImpl(const sup::sequencer::Instruction* instruction) override;
};

#endif  // SUP_SEQUENCER_TEST_UI_H_
